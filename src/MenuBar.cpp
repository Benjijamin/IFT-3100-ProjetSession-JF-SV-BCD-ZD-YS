#include "MenuBar.h"

void MenuBar::setup() { screenCapture.setup(); }
void MenuBar::update() { screenCapture.update(); }
void MenuBar::draw() { screenCapture.draw(); }
void MenuBar::exit() {}

void MenuBar::drawGui() {
	if (ImGui::BeginMainMenuBar()) {

		drawFileMenu();
		drawEditMenu();
		drawRenderMenu();
		drawWindowMenu();

		drawHelpMenu();

		ImGui::EndMainMenuBar();
	}

	drawToolBar();

	dessinVectoriel.drawGui();
	screenCapture.drawGui();
}

void MenuBar::drawFileMenu() {
	if (ImGui::BeginMenu("File")) {
		drawProjectMenu();

		ImGui::Separator();

		drawDrawingMenu();

		ImGui::Separator();

		drawSettingsMenu();

		ImGui::Separator();

		drawExitMenu();

		ImGui::EndMenu();
	}
}

// MÉTHODES POUR 'ONGLET FILE'

void MenuBar::drawProjectMenu() {
	if (ImGui::BeginMenu("Project")) {
		if (ImGui::MenuItem("New Project")) {}
		if (ImGui::MenuItem("Open Project")) {
			ofFileDialogResult result = ofSystemLoadDialog("Choisissez un fichier de projet ...");
			if (result.bSuccess) { openProject(result.getPath()); }
		}
		if (ImGui::MenuItem("Save Project")) {
			ofFileDialogResult result = ofSystemSaveDialog("monProjet.json", "Enregistrer sous ...");
			if (result.bSuccess) { saveProject((result.getPath())); }
		}

		ImGui::EndMenu();
	}
}

void MenuBar::drawDrawingMenu() {
	if (ImGui::BeginMenu("Drawing")) {
		if (ImGui::MenuItem("New Drawing..")) {
			if (onNewDrawing) onNewDrawing();
		}
		if (ImGui::MenuItem("Export Drawing")) {
			ofFileDialogResult result = ofSystemSaveDialog("monImage.png", "Enregistrer sous ...");
			if (result.bSuccess) { imageEditor.save(result.getPath()); }
		}
		if (ImGui::MenuItem("Import Drawing")) {
			ofFileDialogResult result = ofSystemLoadDialog("Choisissez un fichier image a importer ...");
		}

		ImGui::EndMenu();
	}
}

void MenuBar::drawSettingsMenu() {
	if (ImGui::BeginMenu("Settings")) {
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

		ImGui::EndMenu();
	}
}

void MenuBar::drawExitMenu() {
	if (ImGui::MenuItem("Exit")) { 
		ofExit();
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


// MÉTHODES POUR ONGLET 'EDIT'
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


// MÉTHODES POUR ONGLET 'RENDER'
void MenuBar::drawRenderMenu() {
	if (ImGui::BeginMenu("Render")) {
		if (ImGui::MenuItem("Render Image")) {}
		if (ImGui::MenuItem("Render Animation")) {}

		ImGui::EndMenu();
	}
}


// MÉTHODES POUR ONGLET 'WINDOW'
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

// MÉTHODES POUR ONGLET 'HELP'
void MenuBar::drawHelpMenu() {
	if (ImGui::BeginMenu("Help")) {
		if (ImGui::MenuItem("Documentation")) {
			ofLaunchBrowser("https://openframeworks.cc/documentation/");
		}
		ImGui::EndMenu();
	}
}


// MÉTHODES POUR BARRE D'OUTILS VERTICALE
void MenuBar::drawToolBar() {
	ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - 150, 20));
	ImGui::SetNextWindowSize(ImVec2(50, 500));

	ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	if (ImGui::Button("Dessin Vectoriel", ImVec2(100, 40))) { dessinVectoriel.begin(); }

	ImGui::End();
}