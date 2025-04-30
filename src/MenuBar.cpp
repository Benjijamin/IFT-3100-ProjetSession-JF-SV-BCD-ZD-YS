#include "MenuBar.h"

void MenuBar::setup() { 
	screenCapture.setup();

	previewFbo.allocate(200, 200, GL_RGBA);
	previewLight.setup();
	previewLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	previewLight.setPosition(0, 0, 200);
	previewLight.enable();
}
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

	//drawToolBar();

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

		if (ImGui::MenuItem("Export Drawing")) {
			
		}

		if (ImGui::MenuItem("Import Drawing")) {
			
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
	if (ImGui::BeginMenu("Renderer")) {
		if (ImGui::BeginMenu("Classic rendering")) {
			if (ImGui::BeginMenu("Materials")) {
				if (ImGui::MenuItem("Create material")) { 
					showCreateMatWindow = true; 
				}
				if (ImGui::MenuItem("Import material")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (showCreateMatWindow) { drawCreateMaterialWindow(); }
}

void MenuBar::drawCreateMaterialWindow() {
	// Begin une fenêtre ImGui flottante
	ImGui::Begin("Create Material", &showCreateMatWindow,
		ImGuiWindowFlags_AlwaysAutoResize);

	// Nom du matériau
	//ImGui::InputText("Name", &newMatName);
	ImGui::InputText("Name", newMatNameBuf, IM_ARRAYSIZE(newMatNameBuf));

	// Sliders couleur
	float amb[3] = { tmpAmbient.r / 255.0f, tmpAmbient.g / 255.0f, tmpAmbient.b / 255.0f };
	if (ImGui::ColorEdit3("Ambient", amb)) {
		tmpAmbient = ofColor(amb[0] * 255, amb[1] * 255, amb[2] * 255);
	}
	float dif[3] = { tmpDiffuse.r / 255.0f, tmpDiffuse.g / 255.0f, tmpDiffuse.b / 255.0f };
	if (ImGui::ColorEdit3("Diffuse", dif)) {
		tmpDiffuse = ofColor(dif[0] * 255, dif[1] * 255, dif[2] * 255);
	}
	float spec[3] = { tmpSpecular.r / 255.0f, tmpSpecular.g / 255.0f, tmpSpecular.b / 255.0f };
	if (ImGui::ColorEdit3("Specular", spec)) {
		tmpSpecular = ofColor(spec[0] * 255, spec[1] * 255, spec[2] * 255);
	}

	// Slider shininess
	ImGui::SliderFloat("Shininess", &tmpShininess, 1.0f, 128.0f);

	// Autres Sliders (Types de texture, par exemple)
	// ...

	// Boutons Save / Cancel
	if (ImGui::Button("Save")) {
		std::string finalName(newMatNameBuf);
		ofMaterial mat;
		mat.setAmbientColor(tmpAmbient);
		mat.setDiffuseColor(tmpDiffuse);
		mat.setSpecularColor(tmpSpecular);
		mat.setShininess(tmpShininess);
		if (materials) materials->create(finalName, mat);
		showCreateMatWindow = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		showCreateMatWindow = false;
	}

	ImGui::Separator();
	ImGui::Text("Preview:");

	//— Rendu du preview dans le FBO —
	previewFbo.begin();
	ofClear(0);
	ofEnableDepthTest();
	previewCam.begin();
	// appliquer le matériau temporaire
	tmpAmbient.normalize(); tmpDiffuse.normalize(); tmpSpecular.normalize();
	ofSetGlobalAmbientColor(tmpAmbient);
	previewLight.enable();
	ofMaterial tmpMat;
	tmpMat.setAmbientColor(tmpAmbient);
	tmpMat.setDiffuseColor(tmpDiffuse);
	tmpMat.setSpecularColor(tmpSpecular);
	tmpMat.setShininess(tmpShininess);
	tmpMat.begin();
	ofDrawSphere(0, 0, 50, 50);  // sphère de presentation
	tmpMat.end();
	previewLight.disable();
	previewCam.end();
	ofDisableDepthTest();
	previewFbo.end();

	// afficher le FBO dans ImGui
	ImGui::Image((ImTextureID)(uintptr_t)previewFbo.getTexture().getTextureData().textureID,
		ImVec2(200, 200));

	ImGui::End();
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
	/*
	ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - 150, 20));
	ImGui::SetNextWindowSize(ImVec2(50, 500));

	ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	if (ImGui::Button("Dessin Vectoriel", ImVec2(100, 40))) { dessinVectoriel.begin(); }

	ImGui::End();
	*/
}
