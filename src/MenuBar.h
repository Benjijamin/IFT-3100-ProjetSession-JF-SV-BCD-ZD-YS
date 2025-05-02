#pragma once

#include "ofxImGui.h"
#include "ofMain.h"

#include "imageEditor.h"
#include "ScreenCapture.h"
#include "DessinVectoriel.h"
#include "Renderer.h"
#include "Material.h"

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

	std::function<void()> onNewDrawing;

	bool* tonemapping;

	void setMaterial(Materials* m) { materials = m; }
	void setRenderer(Renderer* r) { renderer = r; }

	void drawCreateMaterialWindow();

private:
	// Objets issus d'autres classes pour liaison avec boutons de l'interface
	ScreenCapture screenCapture;


	// ONGLET DES FICHIERS
	//----------------------//
	/*
	* \brief Gère le bouton 'File' de l'interface.
	* \brief La structure du bouton englobe des «sous-boutons» qui sont des méthodes appellées par drawFileMenu():
	* \brief > void drawProjectMenu(); (Gestion des projets)
	* \brief > void drawDrawingMenu();	(Gestion des dessins)
	* \brief > drawSettingsMenu(); (Gestion des paramètres)
	* \brief > drawExitMenu(); (Quitter l'application)
	* \brief > saveProject(); et openProject(); (Sauvegarde globale du projet qui peut contenir plusieurs dessins (plus tard))
	*/
	void drawFileMenu();

	void drawProjectMenu();
	void drawDrawingMenu();
	void drawSettingsMenu();
	void drawExitMenu();
	void saveProject(const std::string& filePath);
	void openProject(const std::string& filePath);

	//----------------------//
	/*
	* \brief Gère le bouton 'Render' de l'interface.
	* \brief La structure du bouton englobe des «sous-boutons» qui sont des méthodes appellées par drawRenderMenu():
	* \brief >
	*/
	void drawRenderMenu();

	//void drawCreateMaterialWindow();
	void loadMaterialFromJson(const std::string& filepath);

	// Fenêtre de modification pour un matériau (version illumination classique)
	bool showCreateMatWindow = false;
	//std::string newMatName = "NewMaterial";
	char newMatNameBuf[64] = "NewMaterial";
	ofColor    tmpAmbient = ofColor(50);
	ofColor    tmpDiffuse = ofColor(200);
	ofColor    tmpSpecular = ofColor(255);
	float      tmpShininess = 32.0f;

	// Preview du matériau
	ofFbo      previewFbo;
	ofEasyCam  previewCam;
	ofLight    previewLight;

	Materials* materials = nullptr;
	Renderer* renderer = nullptr;

	ShaderType previewShader = ShaderType::blinn_phong;

	//----------------------//
	/*
	* \brief Gère le bouton 'Window' de l'interface.
	* \brief La structure du bouton englobe des «sous-boutons» qui sont des méthodes appellées par drawWindowMenu():
	* \brief >
	*/
	void drawWindowMenu();

	//----------------------//
	/*
	* \brief Gère le bouton 'Help' dans l'interface.
	* \brief La structure du bouton englobe des «sous-boutons» qui sont des méthodes appellées par drawHelpMenu():
	* \brief >
	*/
	void drawHelpMenu();
};