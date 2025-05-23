#include "ofMain.h"
#include "ofApp.h"

int main() {
	ofGLWindowSettings settings;
	settings.setSize(1536, 1152);
	settings.setGLVersion(3, 3);
	settings.windowMode = OF_WINDOW;
	
	auto window = ofCreateWindow(settings);
	
	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
