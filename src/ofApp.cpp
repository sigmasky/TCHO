/*
 * Authors:   Francisco Ibarra & Nhat Nguyen
 * Updated:   12.10.2019
 * Professor: Kevin Smith
 * Class:     CS 134
 */

#include "ofApp.h"

void ofApp::setup() {
	// Global ------------------------------------------------------------- /
	// Game State [PRODUCTION, DEVELOPMENT]
	game_state = PRODUCTION;
	debug_mode = false;
	game_score = 0;
	int player1_score = 0;
	int player2_score = 0;
	player1_winner = false;
	player2_winner = false;

	// Window Title
	ofSetWindowTitle("The Car Hockey Olympics - The Final Project");

	// User Experience
	ofEnableDepthTest();
	ofEnableSmoothing();
	ofSetVerticalSync(true);

	// Splash Screen Section ---------------------------------------------- /
	// Splash Screen
	splash_screen_image.load("images/splash_screen.png");
	splash_screen_sound.load("sounds/car_door.wav");
	splash_screen_fade = true;
	fade_amount = 0;


	// Start Screen Section ----------------------------------------------- /
	// Start Screen
	start_screen_image.load("images/start_screen.png");
	start_screen_sound.load("sounds/car_start.wav");
	start_screen_repeat = true;

	// Music
	music.load("music/brazilian_samba_music.mp3");


	// Game Screen Section ------------------------------------------------ /
	// GUI
	gui_time_background.load("images/gui_time_background.png");
	gui_player1_background.load("images/gui_player1_background.png");
	gui_player2_background.load("images/gui_player2_background.png");
	gui_time_font.load(UI_FONT, UI_SIZE);
	gui_player1_font.load(UI_FONT, UI_SIZE);
	gui_player2_font.load(UI_FONT, UI_SIZE);
	gui_fps_font.load(UI_FONT, UI_SIZE + 6);

	// Camera
	// Current Camera
	current_camera = &overworld_camera;
	// Overworld Camera
	overworld_camera.setFov(65.5);
	overworld_camera.setNearClip(0.1);
	overworld_camera.setDistance(1500);
	overworld_camera.setPosition(
		overworld_camera.getPosition().x,
		overworld_camera.getPosition().y + 200,
		overworld_camera.getPosition().z
	);
	// Camera Player 1
	top_camera1.setNearClip(0.1);
	top_camera1.setFov(65.5);
	top_camera1.setPosition(player1.getPosition().x, 1000, player1.getPosition().z);
	top_camera1.lookAt(glm::vec3(player1.getPosition().x, 0, player1.getPosition().z));
	top_camera1.rotate(-90, glm::vec3(0, 1, 0));
	// Camera Player 2
	top_camera2.setNearClip(0.1);
	top_camera2.setFov(65.5);
	top_camera2.setPosition(player2.getPosition().x, 1000, player2.getPosition().z);
	top_camera2.lookAt(glm::vec3(player2.getPosition().x, 0, player2.getPosition().z));
	top_camera2.rotate(90, glm::vec3(0, 1, 0));

	// Winner Images
	player1_winner_image.load("images/player1_winner.png");
	player2_winner_image.load("images/player2_winner.png");
	goal_sound.load("sounds/goal.wav");
	goal_repeat = true;

	// Effects
	cheer_sound.load("sounds/cheer.mp3");
	cheer_repeat = true;

	// Lighting
	// Ambient Light
	ambient_light.setAreaLight(64, 64);
	ambient_light.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	ambient_light.setDiffuseColor(ofFloatColor(1, 1, 1));
	ambient_light.setSpecularColor(ofFloatColor(1, 1, 1));
	ambient_light.setPosition(0, 500, 0);
	ambient_light.rotate(90, ofVec3f(1, 0, 0));

	// Game Objects
	// Player 1: Red car
	player1.loadModel("models/player2.obj");
	player1.setScaleNormalization(false);
	player1.setScale(0.5, 0.5, 0.5);
	player1_particle.radius = 2.0;
	player1_particle.lifespan = 10000;
	player1_particle.damping = 0.5;
	player1_particle.position = ofVec3f(-800, 16, 0);
	setup_thrust_emitter(player1_thrust_emitter);
	// Player 2: Blue car
	player2.loadModel("models/player1.obj");
	player2.setScaleNormalization(false);
	player2.setScale(0.5, 0.5, 0.5);
	player2_particle.radius = 2.0;
	player2_particle.lifespan = 10000;
	player2_particle.damping = 0.5;
	player2_particle.position = ofVec3f(800, 16, 0);
	setup_thrust_emitter(player2_thrust_emitter);
	player2_thrust_emitter.particleColor = ofColor::blue;
	// Puck
	puck.loadModel("models/puck.obj");
	puck.setScaleNormalization(false);
	puck.setScale(0.5, 0.5, 0.5);
	puck.setPosition(0, 16, 0);
	// Puck emitter
	puck_emitter.setRate(1.0);
	puck_emitter.setOneShot(true);
	puck_emitter.setLifespan(100);
	puck_emitter.setParticleRadius(0.01);
	puck_emitter.setPosition(ofVec3f(0, 500, 0));
	puck_emitter.setVelocity(ofVec3f(0, -300, 0));
	ParticleSystem* sys = puck_emitter.sys;
	gravity_force.set(ofVec3f(0, -100.0, 0));
	sys->addForce(&gravity_force);
	sys->addForce(new TurbulenceForce(ofVec3f(-3, -1, -1), ofVec3f(3, 1, 1)));
	// Ground
	ground.loadModel("models/ground.obj");
	ground.setScaleNormalization(false);
	ground.setScale(500, 10, 500);
	ground.setPosition(0, 0, 0);

	// Octree
	terrain_mesh = ground.getMesh(0);
	number_of_levels = 6;
	octree.create(terrain_mesh, number_of_levels);
	draw_mode = false;

	// Forces for both players
	turbForce = new TurbulenceForce(ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10));

	// Forces for player 1
	player1_impulse_force = new ImpulseForce();
	player1_thrust_force = new ThrusterForce();

	// Forces for player 2
	player2_impulse_force = new ImpulseForce();
	player2_thrust_force = new ThrusterForce();

	// Forces for puck
	puck_impulse_force = new ImpulseForce();
	puck_thrust_force = new ThrusterForce();

	// Particle system for player1
	player1_particle_system.add(player1_particle);
	player1_particle_system.addForce(turbForce);
	player1_particle_system.addForce(player1_thrust_force);
	player1_particle_system.addForce(player1_impulse_force);

	// Particle system for player2
	player2_particle_system.add(player2_particle);
	player2_particle_system.addForce(turbForce);
	player2_particle_system.addForce(player2_thrust_force);
	player2_particle_system.addForce(player2_impulse_force);
}

void ofApp::setup_thrust_emitter(ParticleEmitter& thrustEmitter) {
	thrustEmitter.setVelocity(ofVec3f(0, 0, 0));
	thrustEmitter.setOneShot(true);
	thrustEmitter.setEmitterType(DiscEmitter);
	thrustEmitter.setGroupSize(10);
	thrustEmitter.setLifespan(1);
	thrustEmitter.visible = false;
	thrustEmitter.setRate(1.0);
	thrustEmitter.setParticleRadius(20);
	thrustEmitter.radius = 20;
	thrustEmitter.sys->addForce(new ImpulseRadialForce(50.0));
}
void ofApp::update() {
	// Splash Screen: Fade Effect
	if (game_state == "splash_screen") {
		if (splash_screen_fade) {
			if (fade_amount > 200) {
				splash_screen_fade = false;
			}
			fade_amount += 2;
		}
		else {
			if (fade_amount < 0) {
				game_state = "start_screen";
			}
			fade_amount -= 6;
		}
	}

	// Game Screen: Camera
	if (game_state == "game_screen") {
		top_camera1.setPosition(player1.getPosition().x, 1000, player1.getPosition().z);
		top_camera2.setPosition(player2.getPosition().x, 1000, player2.getPosition().z);
	}

	// Game Screen: Controls
	int speed = 1000;
	update_movement(speed, 1, player1_controls, player1_thrust_force, player1_thrust_emitter);
	update_movement(speed, 2, player2_controls, player2_thrust_force, player2_thrust_emitter);

	player1_particle_system.update();
	player1.setPosition(
		player1_particle_system.particles[0].position.x,
		player1_particle_system.particles[0].position.y,
		player1_particle_system.particles[0].position.z
	);

	player2_particle_system.update();
	player2.setPosition(
		player2_particle_system.particles[0].position.x,
		player2_particle_system.particles[0].position.y,
		player2_particle_system.particles[0].position.z
	);

	ofVec3f player1_position = player1.getPosition();
	ofVec3f player2_position = player2.getPosition();

	//if (player1_position.x > 2400 || player1_position.x < -2400 
	//	|| player1_position.z > 2400 || player1_position.z < -2400) {
	//	player1_particle_system.particles[0].position = ofVec3f(0, 16, 0);
	//}
	//if (player2_position.x > 2400 || player2_position.x < -2400
	//	|| player2_position.z > 2400 || player2_position.z < -2400) {
	//	player2_particle_system.particles[0].position = ofVec3f(0, 16, 0);
	//}

	if (puck_emitter.sys->particles.size() > 0) {
		puck.setPosition(
			puck_emitter.sys->particles[0].position.x,
			puck_emitter.sys->particles[0].position.y,
			puck_emitter.sys->particles[0].position.z
		);
	}
	else {
		puck.setPosition(0, 16, 0);
	}

	// Check collisions
	if (player1PuckCollides()) {
		player1_score++;
		if (cheer_repeat) {
			cheer_sound.play();
			cheer_repeat = false;
		}
		randomizePuckPosition(1000);
	}
	if (player2PuckCollides()) {
		player2_score++;
		if (cheer_repeat) {
			cheer_sound.play();
			cheer_repeat = false;
		}
		randomizePuckPosition(1000);
	}

	puck_emitter.update();
	checkPuckGroundCollisions();
	// Winner conditions
	if (player1_score >= 5) {
		player1_winner = true;
	}
	else if (player2_score >= 5) {
		player2_winner = true;
	}

	player1_thrust_emitter.update();
	player2_thrust_emitter.update();
}

void ofApp::draw() {
	if (game_state == "splash_screen") {
		ofBackground(ofColor::black);
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, fade_amount);
		if (fade_amount == 50) splash_screen_sound.play();
		splash_screen_image.draw(0, 0);
		ofDisableAlphaBlending();
	}
	else if (game_state == "start_screen") {
		ofSetColor(ofColor::white);
		if (start_screen_repeat) {
			start_screen_sound.play();
			start_screen_repeat = false;
			music.setLoop(true);
			music.play();
			music.setVolume(0.25f);
		}
		start_screen_image.draw(0, 0);
	}
	else if (game_state == "game_screen") {
		// GUI
		ofBackground(ofColor::skyBlue);
		ofSetColor(ofColor::white);
		ofDisableLighting();
		ambient_light.disable();
		gui_player1_font.drawString(to_string(player1_score), 184, 112);
		gui_player2_font.drawString(to_string(player2_score), 1730, 112);
		gui_time_font.drawString(to_string((int)ofGetElapsedTimef()) + "s", 184, 202);
		gui_player1_background.draw(1560, 48);
		gui_player2_background.draw(36, 48);
		gui_time_background.draw(36, 138);
		gui_fps_font.drawString("FPS: " + to_string((int)ofGetFrameRate()), 36, 264);

		// Winner
		if (player1_winner) {
			player1_winner_image.draw(0, 0);
			if (goal_repeat) {
				goal_sound.play();
				goal_repeat = false;
			}
		} else if (player2_winner) {
			  player2_winner_image.draw(0, 0);
			  if (goal_repeat) {
				  goal_sound.play();
				  goal_repeat = false;
			 }
		}

		// Lighting
		ofEnableLighting();
		ambient_light.enable();

		// Camera
		current_camera->begin();

		// Debug (button 'e' for on/off)
		if (debug_mode) {
			// Light
			ambient_light.draw();

			// Puck
			ofNoFill();
			ofSetColor(ofColor::purple);
			int puck_height = 16;
			int puck_size = 50;
			ofDrawBox(
				glm::vec3(puck.getPosition().x, puck.getPosition().y / 2, puck.getPosition().z),
				puck_size + 2.5,
				puck_height,
				puck_size - 2.5
			);
			// Players
			ofSetColor(ofColor::purple);
			int player_width = 75;
			int player_height = 16;
			int player_depth = 150;
			ofDrawBox(
				glm::vec3(player1.getPosition().x, player1.getPosition().y / 2, player1.getPosition().z),
				player_depth,
				player_height,
				player_width
			);
			ofDrawBox(
				glm::vec3(player2.getPosition().x, player2.getPosition().y / 2, player2.getPosition().z),
				player_depth,
				player_height,
				player_width
			);
			ofFill();

		}
		else {
			// Write Code Here ----------------------------------------------------------------- /
			player1.drawFaces();
			player2.drawFaces();
			player1_thrust_emitter.draw();
			player2_thrust_emitter.draw();
			puck.drawFaces();
			ground.drawFaces();
			puck_emitter.draw();


			// --------------------------------------------------------------------------------- /
		}


		current_camera->end();
	}
}

void ofApp::keyPressed(int key) {
	// Quits the game
	if (key == 'q') std::exit(1);
	// Space: Transition from start screen to game screen
	if (game_state == "start_screen" && key == ' ') game_state = "game_screen";

	// Game Screen: Debug mode (button 'e' for on/off)
	if (game_state == "game_screen" && key == 'e') debug_mode ? debug_mode = false : debug_mode = true;

	// Game Screen: Cameras
	if (game_state == "game_screen" && key == '1') current_camera = &overworld_camera;
	if (game_state == "game_screen" && key == '2') current_camera = &top_camera1;
	if (game_state == "game_screen" && key == '3') current_camera = &top_camera2;

	// Game Controls
	if (game_state == "game_screen") {
		switch (key) {
		case OF_KEY_UP:
			player1_controls.down = true;
			break;
		case OF_KEY_DOWN:
			player1_controls.up = true;
			break;
		case OF_KEY_RIGHT:
			player1_controls.right = true;
			break;
		case OF_KEY_LEFT:
			player1_controls.left = true;
			break;
		case 'w':
			player2_controls.up = true;
			break;
		case 's':
			player2_controls.down = true;
			break;
		case 'd':
			player2_controls.right = true;
			break;
		case 'a':
			player2_controls.left = true;
			break;
		case 'i':
			puck_controls.up = true;
			break;
		case 'k':
			puck_controls.down = true;
			break;
		case 'l':
			puck_controls.right = true;
			break;
		case 'j':
			puck_controls.left = true;
			break;
		}
	}
}
void ofApp::keyReleased(int key) {
	// Game Controls
	if (game_state == "game_screen") {
		switch (key) {
		case OF_KEY_UP:
			player1_controls.down = false;
			break;
		case OF_KEY_DOWN:
			player1_controls.up = false;
			break;
		case OF_KEY_RIGHT:
			player1_controls.right = false;
			break;
		case OF_KEY_LEFT:
			player1_controls.left = false;
			break;
		case 'w':
			player2_controls.up = false;
			break;
		case 's':
			player2_controls.down = false;
			break;
		case 'd':
			player2_controls.right = false;
			break;
		case 'a':
			player2_controls.left = false;
			break;
		case 'i':
			puck_controls.up = false;
			break;
		case 'k':
			puck_controls.down = false;
			break;
		case 'l':
			puck_controls.right = false;
			break;
		case 'j':
			puck_controls.left = false;
			break;
		}
	}
}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}

void ofApp::checkPuckGroundCollisions() {
	// From bouncing ball example
	for (int i = 0; i < puck_emitter.sys->particles.size(); i++) {
		ofVec3f vel = puck_emitter.sys->particles[i].velocity;
		if (vel.y >= 0)
			break;
		ofVec3f pos = puck_emitter.sys->particles[i].position;
		restitution = 0.85;
		float puck_height = 32;
		if (pos.y < puck_height / 2) {
			ofVec3f norm = ofVec3f(0, 1, 0);
			ofVec3f f = (restitution + 1.0) * ((-vel.dot(norm)) * norm);
			puck_emitter.sys->particles[i].forces += ofGetFrameRate() * f;
		}
	}
}

void ofApp::update_movement(int speed, int player, player_controls& controls, ThrusterForce* thrust_force, ParticleEmitter& thrust_emitter) {

	if (controls.up) {
		thrust_force->add(ofVec3f(-speed, 0, 0));
		thrust_emitter.setVelocity(ofVec3f(-100, 0, 0));
		player == 1 ? thrust_emitter.setPosition(player1.getPosition()) : thrust_emitter.setPosition(player2.getPosition());
		player == 1 ? activate_player1_boost() : activate_player2_boost();
	}
	else if (controls.down) {
		thrust_force->add(ofVec3f(speed, 0, 0));
		thrust_emitter.setVelocity(ofVec3f(100, 0, 0));
		player == 1 ? thrust_emitter.setPosition(player1.getPosition()) : thrust_emitter.setPosition(player2.getPosition());
		player == 1 ? activate_player1_boost() : activate_player2_boost();
	}
	else {
		if (thrust_force->getCurrentForce().x < 0)
			thrust_force->add(ofVec3f(speed, 0, 0));
		else if (thrust_force->getCurrentForce().x > 0)
			thrust_force->add(ofVec3f(-speed, 0, 0));
	}
	if (controls.right) {
		thrust_force->add(ofVec3f(0, 0, -speed));
		thrust_emitter.setVelocity(ofVec3f(0, 0, -100));
		player == 1 ? thrust_emitter.setPosition(player1.getPosition()) : thrust_emitter.setPosition(player2.getPosition());
		player == 1 ? activate_player1_boost() : activate_player2_boost();
	}
	else if (controls.left) {
		thrust_force->add(ofVec3f(0, 0, speed));
		thrust_emitter.setVelocity(ofVec3f(0, 0, 100));
		player == 1 ? thrust_emitter.setPosition(player1.getPosition()) : thrust_emitter.setPosition(player2.getPosition());
		player == 1 ? activate_player1_boost() : activate_player2_boost();
	}
	else {
		if (thrust_force->getCurrentForce().z < 0)
			thrust_force->add(ofVec3f(0, 0, speed));
		else if (thrust_force->getCurrentForce().z > 0)
			thrust_force->add(ofVec3f(0, 0, -speed));
	}
}

bool ofApp::playersCollide() {
	// Returns true if two players collide
	bool x_collide = player1.getPosition().x - 75 <= player2.getPosition().x + 75 &&
		player1.getPosition().x + 75 >= player2.getPosition().x - 75;
	bool y_collide = player1.getPosition().y - 8 <= player2.getPosition().y + 8 &&
		player1.getPosition().y + 8 >= player2.getPosition().y - 8;
	bool z_collide = player1.getPosition().z - 37.5 <= player2.getPosition().z + 37.5 &&
		player1.getPosition().z + 37.5 >= player2.getPosition().z - 37.5;
	return x_collide && y_collide && z_collide;
}

bool ofApp::player1PuckCollides() {
	// Returns true if player 1 and puck collides
	bool x_collide = player1.getPosition().x - 75 <= puck.getPosition().x + 25 &&
		player1.getPosition().x + 75 >= puck.getPosition().x - 25;
	bool y_collide = player1.getPosition().y - 16 <= puck.getPosition().y + 16 &&
		player1.getPosition().y + 16 >= puck.getPosition().y - 16;
	bool z_collide = player1.getPosition().z - 37.5 <= puck.getPosition().z + 25 &&
		player1.getPosition().z + 37.5 >= puck.getPosition().z - 25;
	return x_collide && y_collide && z_collide;
}

bool ofApp::player2PuckCollides() {
	// Returns true if player 2 and puck collides
	bool x_collide = player2.getPosition().x - 75 <= puck.getPosition().x + 25 &&
		player2.getPosition().x + 75 >= puck.getPosition().x - 25;
	bool y_collide = player2.getPosition().y - 16 <= puck.getPosition().y + 16 &&
		player2.getPosition().y + 16 >= puck.getPosition().y - 16;
	bool z_collide = player2.getPosition().z - 37.5 <= puck.getPosition().z + 25 &&
		player2.getPosition().z + 37.5 >= puck.getPosition().z - 25;
	return x_collide && y_collide && z_collide;
}

void ofApp::randomizePuckPosition(int p) {
	float x_random = (rand() % (2 * p + 1)) + (-p);
	float z_random = (rand() % (2 * p + 1)) + (-p);
	float y = 400;

	puck_emitter.sys->particles.clear();
	puck_emitter.setPosition(ofVec3f(x_random, y, z_random));
	puck_emitter.start();
}