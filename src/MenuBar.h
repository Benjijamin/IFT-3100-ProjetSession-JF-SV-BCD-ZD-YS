#pragma once

#include "ofxImGui.h"
#include "ofMain.h"

#include "imageEditor.h"
#include "ScreenCapture.h"
#include "DessinVectoriel.h"
#include "SceneEditor.h"

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

    std::function<void()> onNewDrawing;

private:
	// Objets issus d'autres classes pour liaison avec boutons de l'interface
	ScreenCapture screenCapture;
	ImageEditor imageEditor;
	DessinVectoriel dessinVectoriel;
	SceneEditor sceneEditor;

	// Methodes pour les boutons de menu
	void drawFileMenu();
	void drawEditMenu();
	void drawSettingsMenu();
	void drawRenderMenu();
	void drawWindowMenu();
	void drawSceneMenu();
	void drawHelpMenu();
	void drawToolBar();
};