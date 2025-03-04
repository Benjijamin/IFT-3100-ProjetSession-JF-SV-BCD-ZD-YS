#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include "imageEditor.h"
#include "ScreenCapture.h"

/*
* \brief Classe qui gère la barre de menu.
*/
class MenuBar {

public:
	void setup();
	void update();
	void draw();
	void drawGui();
	void exit();



	void saveProject(const std::string& filePath);
	void openProject(const std::string& filePath);

	ScreenCapture screenCapture;
	ImageEditor imageEditor;

private:
	void drawFileMenu();
	void drawEditMenu();
	void drawSettingsMenu();
	void drawRenderMenu();
	void drawWindowMenu();
	void drawHelpMenu();
};