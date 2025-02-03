#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		//Couleur qui est pris du colorPicker
		float pickedColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	private:
		bool showColorPicker = false;
		int colorPickerDisplay = 1;

		void menuBar();
		void colorPicker();
};
