#include "MenuBar.h"

void MenuBar::setup() { screenCapture.setup(); }

void MenuBar::update() { screenCapture.update(); }

void MenuBar::draw() { screenCapture.draw(); }

void MenuBar::drawGui() {
	if (ImGui::BeginMainMenuBar()) {

		drawFileMenu();
		drawEditMenu();
		drawRenderMenu();
		drawWindowMenu();

		drawHelpMenu();

		ImGui::EndMainMenuBar();
	}
	screenCapture.drawGui();
}

/*
* ONGLET DES FICHIERS
*
* ---Contenu---
* >Nouveau projet
* >Ouvrir un projet sauvegardé ou existant (bin/data)
* >Sauvegarder un projet dans un fichier (bin/data)
* >Import/Export (pour image ou autre)
* >Paramètres de l'interface (si besoin)
* >Quitter l'interface
*/
void MenuBar::drawFileMenu() {
	if (ImGui::BeginMenu("File")) {

		if (ImGui::MenuItem("New Project")) {}

		if (ImGui::MenuItem("Open Project")) {
			ofFileDialogResult result = ofSystemLoadDialog("Choisissez un fichier de projet");
			if (result.bSuccess) { openProject(result.getPath()); }
		}

		if (ImGui::MenuItem("Save Project")) {
			ofFileDialogResult result = ofSystemSaveDialog("monProjet.json", "Enregistrer sous ...");
			if (result.bSuccess) { saveProject((result.getPath())); }
		}

		if (ImGui::MenuItem("Import")) {
			ofFileDialogResult result = ofSystemLoadDialog("Choisissez une image à charger");
			if (result.bSuccess) { imageEditor.load(result.getPath()); }
		}

		if (ImGui::MenuItem("Export")) {
			ofFileDialogResult result = ofSystemSaveDialog("monImage.png", "Enregistrer sous ...");
			if (result.bSuccess) { imageEditor.save(result.getPath()); }
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Settings")) { 
			drawSettingsMenu();
			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Exit")) { ofExit(); }

		ImGui::EndMenu();
	}
}

void MenuBar::drawSettingsMenu() {
	if (ImGui::BeginMenu("Themes")) {
		if (ImGui::MenuItem("Classic Theme")) {
			ImGui::StyleColorsClassic();
			ofSetBackgroundColor(ofColor(60, 60, 60));
		}

		if (ImGui::MenuItem("Light Theme")) {
			ImGui::StyleColorsLight();
			ofSetBackgroundColor(ofColor(230, 242, 255));
		}

		if (ImGui::MenuItem("Dark Theme")) {
			ImGui::StyleColorsDark();
			ofSetBackgroundColor(ofColor(32, 32, 32));
		}

		ImGui::EndMenu();
	}
}

/*
* ONGLET DE L'ÉDITEUR
*
* ---Contenu---
*
* >Undo/redo/delete/etc.
* >Déplacement des primitives
* >Modification des primitives
* >Autres
*/
void MenuBar::drawEditMenu() {
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Capture Screenshot")) { screenCapture.captureScreenshot(); }

		ImGui::Separator();

		if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
		if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}

		ImGui::Separator();

		if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
		if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
		if (ImGui::MenuItem("Paste", "Ctrl+V")) {}

		ImGui::EndMenu();
	}
}

/*
* ONGLET DU RENDEUR D'IMAGE
*
* ---Contenu---
* >
*/
void MenuBar::drawRenderMenu() {
	if (ImGui::BeginMenu("Render")) {
		if (ImGui::MenuItem("Render Image")) {}
		if (ImGui::MenuItem("Render Animation")) {}

		ImGui::EndMenu();
	}
}

/*
* ONGLET DE FENÊTRE
*
* ---Contenu---
* >Plein écran
* >Séparer le canva en deux
* >Rétablir le layout original
*/
void MenuBar::drawWindowMenu() {
	if (ImGui::BeginMenu("Window")) {
		if (ImGui::MenuItem("Toggle Fullscreen")) { ofToggleFullscreen(); }
		if (ImGui::MenuItem("Windowed")) { ofSetFullscreen(false); }
		if (ImGui::MenuItem("Split canva")) {}

		ImGui::Separator();

		if (ImGui::MenuItem("Reset Layout")) {}

		ImGui::EndMenu();
	}
}

void MenuBar::drawHelpMenu() {
	if (ImGui::BeginMenu("Help")) {
		if (ImGui::MenuItem("Documentation")) {
			ofLaunchBrowser("https://openframeworks.cc/documentation/");
		}
		ImGui::EndMenu();
	}
}

void MenuBar::saveProject(const std::string& filePath) {
	ofFile file(filePath);
	if (!file.exists()) {
		ofLogError() << "Fichier non trouvé : " << filePath;
		return;
	}
}

void MenuBar::openProject(const std::string& filePath) {
	ofFile file(filePath);
	if (!file.exists()) {
		ofLogError() << "Fichier non trouvé : " << filePath;
		return;
	}
}