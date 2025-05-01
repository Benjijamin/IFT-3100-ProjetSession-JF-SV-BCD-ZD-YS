#include "MenuBar.h"

void MenuBar::setup() { screenCapture.setup(); }
void MenuBar::update() { screenCapture.update(); }
void MenuBar::draw() { screenCapture.draw(); }
void MenuBar::exit() {}

void MenuBar::drawGui() {
	if (ImGui::BeginMainMenuBar()) {
		drawFileMenu();
		drawRenderMenu();
		drawWindowMenu();
		drawHelpMenu();

		ImGui::EndMainMenuBar();
	}

	screenCapture.drawGui();
}

void MenuBar::drawFileMenu() {
    if (ImGui::BeginMenu("File")) {
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
		if (ImGui::MenuItem("New Drawing")) {
			if (onNewDrawing) onNewDrawing();
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

// MÉTHODES POUR ONGLET 'RENDER'
void MenuBar::drawRenderMenu() {
	if (ImGui::BeginMenu("Render")) {
		
		if (ImGui::MenuItem("Capture Screenshot")) { screenCapture.captureScreenshot(); }

		ImGui::Separator();

		ImGui::Checkbox("ACES Tone Mapping", tonemapping);

		ImGui::EndMenu();
	}
}


// MÉTHODES POUR ONGLET 'WINDOW'
void MenuBar::drawWindowMenu() {
	if (ImGui::BeginMenu("Window")) {
		if (ImGui::MenuItem("Toggle Fullscreen")) { ofToggleFullscreen(); }
		if (ImGui::MenuItem("Windowed")) { ofSetFullscreen(false); }

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