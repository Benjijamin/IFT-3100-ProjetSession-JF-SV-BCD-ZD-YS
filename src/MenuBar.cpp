#include "MenuBar.h"

void MenuBar::setup() { 
	screenCapture.setup();
}
void MenuBar::update() { screenCapture.update(); }
void MenuBar::draw() { 
	screenCapture.draw();
}
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
	if (ImGui::BeginMenu("Renderer")) {
		if (ImGui::BeginMenu("Classic rendering")) {
			if (ImGui::BeginMenu("Materials")) {
				if (ImGui::MenuItem("Create material")) {
					showCreateMatWindow = true;
				}
				if (ImGui::MenuItem("Import material")) {
					ofFileDialogResult result = ofSystemLoadDialog("Select a material (.json)", false);
					if (result.bSuccess) {
						loadMaterialFromJson(result.getPath());
						showCreateMatWindow = true;
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Shaders")) {

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Capture Screenshot")) { screenCapture.captureScreenshot(); }

		ImGui::Separator();

		ImGui::Checkbox("ACES Tone Mapping", tonemapping);


		ImGui::EndMenu();
	}
	if (showCreateMatWindow) { drawCreateMaterialWindow(); }
}

void MenuBar::drawCreateMaterialWindow() {
	ImGui::Begin("Create Material", &showCreateMatWindow, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputText("Name", newMatNameBuf, IM_ARRAYSIZE(newMatNameBuf));

	float amb[3] = { tmpAmbient.r / 255.0f, tmpAmbient.g / 255.0f, tmpAmbient.b / 255.0f };
	ImGui::ColorEdit3("Ambient", amb);
	tmpAmbient.set(amb[0] * 255, amb[1] * 255, amb[2] * 255);

	float dif[3] = { tmpDiffuse.r / 255.0f, tmpDiffuse.g / 255.0f, tmpDiffuse.b / 255.0f };
	ImGui::ColorEdit3("Diffuse", dif);
	tmpDiffuse.set(dif[0] * 255, dif[1] * 255, dif[2] * 255);

	float spec[3] = { tmpSpecular.r / 255.0f, tmpSpecular.g / 255.0f, tmpSpecular.b / 255.0f };
	ImGui::ColorEdit3("Specular", spec);
	tmpSpecular.set(spec[0] * 255, spec[1] * 255, spec[2] * 255);

	ImGui::SliderFloat("Shininess", &tmpShininess, 1.0f, 128.0f);

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
	if (ImGui::Button("Cancel")) showCreateMatWindow = false;

	ImGui::Separator();
	ImGui::Text("Preview:");

	if (!previewFbo.isAllocated()) {
		previewFbo.allocate(200, 200, GL_RGBA);
		previewCam.setupPerspective(false);
	}

	static const char* shaderNames[] = { "Color Fill", "Lambert", "Gouraud", "Phong", "Blinn-Phong", "Toon" };
	int idx = (int)previewShader;
	if (ImGui::Combo("Shader", &idx, shaderNames, IM_ARRAYSIZE(shaderNames))) { previewShader = (ShaderType)idx; }

	previewFbo.begin();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofEnableDepthTest();
	previewCam.begin();

	
	ofMaterial previewMat;
	previewMat.setAmbientColor(ofFloatColor(tmpAmbient));
	previewMat.setDiffuseColor(ofFloatColor(tmpDiffuse));
	previewMat.setSpecularColor(ofFloatColor(tmpSpecular));
	previewMat.setShininess(tmpShininess);
	
	ofShader* sh = nullptr;

	switch (previewShader) {
	case ShaderType::color_fill:   sh = &renderer->shader_color_fill;   break;
	case ShaderType::lambert:      sh = &renderer->shader_lambert;      break;
	case ShaderType::gouraud:      sh = &renderer->shader_gouraud;      break;
	case ShaderType::phong:        sh = &renderer->shader_phong;        break;
	case ShaderType::blinn_phong:  sh = &renderer->shader_blinn_phong;  break;
	case ShaderType::toon:         sh = &renderer->shader_toon;         break;
	default:                       sh = &renderer->shader_phong;        break;
	}

	if (sh && materials) {
		materials->create("preview", previewMat);

		renderer->setCurrentMaterial("preview");

		renderer->shader = sh;

		sh->begin();
		renderer->shaderAttributesDynamicM();
		ofDrawSphere(0, 0, 0, 50);
		sh->end();
	}

	previewCam.end();
	ofDisableDepthTest();
	previewFbo.end();

	//ImGui::Image((ImTextureID)(uintptr_t)previewFbo.getTexture().getTextureData().textureID, ImVec2(200, 200));
	ImGui::Image((ImTextureID)(uintptr_t)previewFbo.getTexture().getTextureData().textureID, ImVec2(previewFbo.getWidth(), previewFbo.getHeight()));
	ImGui::End();
}

void MenuBar::loadMaterialFromJson(const std::string& filepath) {
	ofJson j;
	try {
		j = ofLoadJson(filepath);
	}
	catch (...) {
		ofLogError("MenuBar") << "Erreur lors du chargement du JSON : " << filepath;
		return;
	}

	if (!j.contains("ambient") || !j.contains("diffuse") || !j.contains("specular") || !j.contains("shininess")) {
		ofLogError("MenuBar") << "Fichier JSON invalide pour un matériau.";
		return;
	}

	// Appliquer les paramètres au buffer actuel
	tmpAmbient.set(j["ambient"]["r"].get<float>() * 255,
		j["ambient"]["g"].get<float>() * 255,
		j["ambient"]["b"].get<float>() * 255);

	tmpDiffuse.set(j["diffuse"]["r"].get<float>() * 255,
		j["diffuse"]["g"].get<float>() * 255,
		j["diffuse"]["b"].get<float>() * 255);

	tmpSpecular.set(j["specular"]["r"].get<float>() * 255,
		j["specular"]["g"].get<float>() * 255,
		j["specular"]["b"].get<float>() * 255);

	tmpShininess = j["shininess"].get<float>();

	// Remplir automatiquement le nom (à partir du nom de fichier)
	std::string filename = ofFilePath::getBaseName(filepath);
	strncpy(newMatNameBuf, filename.c_str(), IM_ARRAYSIZE(newMatNameBuf));
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