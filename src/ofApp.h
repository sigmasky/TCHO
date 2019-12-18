#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ofxAssimpModelLoader.h"
#include "Octree.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	// Added Code: START --------------------------------------------------------------------------- /
	void ofApp::checkPuckGroundCollisions();
	bool ofApp::playersCollide();
	bool ofApp::player1PuckCollides();
	bool ofApp::player2PuckCollides();
	void ofApp::randomizePuckPosition(int p);

	// Global
	string PRODUCTION = "splash_screen";
	string DEVELOPMENT = "game_screen";
	string game_state;
	bool debug_mode;
	int game_score;
	int player1_score;
	int player2_score;
	bool player1_winner;
	bool player2_winner;


	// Splash Screen
	ofImage splash_screen_image;
	ofSoundPlayer splash_screen_sound;
	bool splash_screen_fade;
	int fade_amount;


	// Start Screen
	ofImage start_screen_image;
	ofSoundPlayer start_screen_sound;
	bool start_screen_repeat;
	ofSoundPlayer music;


	// Game Screen
	// GUI
	string UI_FONT = "fonts/SourceSansPro-Semibold.ttf";
	int UI_SIZE = 24;
	ofImage gui_time_background;
	ofImage gui_player1_background;
	ofImage gui_player2_background;
	ofTrueTypeFont gui_time_font;
	ofTrueTypeFont gui_player1_font;
	ofTrueTypeFont gui_player2_font;
	ofTrueTypeFont gui_fps_font;

	// Other
	ofLight ambient_light;			// Lighting
	// Camera
	ofCamera *current_camera;
	ofEasyCam overworld_camera;
	ofCamera top_camera1;
	ofCamera top_camera2;

	// Winner
	ofImage player1_winner_image;
	ofImage player2_winner_image;
	ofSoundPlayer goal_sound;
	bool goal_repeat;

	// Effects
	ofSoundPlayer cheer_sound;
	bool cheer_repeat;

	/*
	CAR PARAMETERS:
		FRONT (x) 150
		HEIGHT (y) 64
		DEPTH (z) 300
	*/
	ofxAssimpModelLoader player1;	// #ff0000
	ofxAssimpModelLoader player2;	// #3c00ff
	ofxAssimpModelLoader puck;
	ofxAssimpModelLoader ground;

	struct player_controls {
		bool up;
		bool down;
		bool right;
		bool left;
	};

	struct player_controls player1_controls = { false, false, false, false };
	struct player_controls player2_controls = { false, false, false, false };
	struct player_controls puck_controls = { false, false, false, false };

	float restitution = 0.85;

	// Player 1 physics variables
	Particle player1_particle;
	ParticleSystem player1_particle_system;
	ParticleEmitter player1_thrust_emitter;
	ThrusterForce* player1_thrust_force;
	ImpulseForce* player1_impulse_force;

	// Player 2 physics variables
	Particle player2_particle;
	ParticleSystem player2_particle_system;
	ParticleEmitter player2_thrust_emitter;
	ThrusterForce* player2_thrust_force;
	ImpulseForce* player2_impulse_force;

	// Puck physics variables
	Particle puck_particle;
	ParticleEmitter puck_emitter;
	ThrusterForce* puck_thrust_force;
	ImpulseForce* puck_impulse_force;

	// Forces for all objects (puck, p1, and p2)
	TurbulenceForce* turbForce;
	GravityForce gravity_force;

	void ofApp::setup_thrust_emitter(ParticleEmitter&);
	void ofApp::update_movement(int speed, int player, player_controls& controls, ThrusterForce* thrust_force, ParticleEmitter& thrust_emitter);
	void ofApp::activate_player1_boost() {
		player1_thrust_emitter.sys->reset();
		player1_thrust_emitter.start();
	}
	void ofApp::activate_player2_boost() {
		player2_thrust_emitter.sys->reset();
		player2_thrust_emitter.start();
	}
	// Octree Variables
	ofMesh terrain_mesh;
	ofEasyCam camera;
	Octree octree;
	int number_of_levels;
	bool draw_mode;

	// Ray Variables
	bool is_ray_intersecting;
	ofVec3f sphere_position;
	TreeNode selected_node;

	bool is_mouse_dragged;
	ofVec3f mouse_position;

	ofVec3f ray_point;
	ofVec3f ray_direction;
	Ray mouse_ray;
	// Added Code: END ----------------------------------------------------------------------------- /

};
