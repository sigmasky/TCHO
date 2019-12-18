#include "ofMain.h"
#include "ofApp.h"

int main() {
	// Full HD or 2K resolution for 16:10
	ofSetupOpenGL(1920, 1200, OF_WINDOW);
	ofRunApp(new ofApp());
}
