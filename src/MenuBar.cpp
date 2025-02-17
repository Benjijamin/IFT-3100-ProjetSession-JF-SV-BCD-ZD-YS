#include "MenuBar.h"

void MenuBar::drawGui() 
{
	if (ImGui::BeginMainMenuBar()) {
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
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("New Project")) {}

			if (ImGui::MenuItem("Open Project")) {
				ofFileDialogResult result = ofSystemLoadDialog("Choisissez un fichier de projet");
				if (result.bSuccess) {
					openProject(result.getPath());
				}

			}
			if (ImGui::MenuItem("Save Project")) {
				ofFileDialogResult result = ofSystemSaveDialog("monProjet.json","Enregistrer sous ...");
				if (result.bSuccess) {
					saveProject((result.getPath()));
				}
				
				
			}
			
			if (ImGui::MenuItem("Import")) {
				ofFileDialogResult result = ofSystemLoadDialog("Choisissez une image à charger");
				if (result.bSuccess) {
					imageEditor.load(result.getPath());
				}
			}

			if (ImGui::MenuItem("Export")) {
				ofFileDialogResult result = ofSystemSaveDialog("monImage.png", "Enregistrer sous ...");
				if (result.bSuccess) {
					imageEditor.save(result.getPath());
				}
			}



			ImGui::Separator();

			if (ImGui::MenuItem("Settings")) {}
			if (ImGui::MenuItem("Exit")) {
			ofExit();
			}

			ImGui::EndMenu();
		}

		/*
		* ONGLET DE L'ÉDITEUR
		* 
		* ---Contenu---
		* >Undo/redo/delete/etc.
		* >Déplacement des primitives
		* >Modification des primitives
		* >Autres
		*/
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}

			ImGui::Separator();

			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {}

			ImGui::EndMenu();
		}

		/*
		* ONGLET DU RENDEUR D'IMAGE
		*
		* ---Contenu---
		* >
		*/
		if (ImGui::BeginMenu("Render")) {
			if (ImGui::MenuItem("Render Image")) {}
			if (ImGui::MenuItem("Render Animation")) {}

			ImGui::EndMenu();
		}

		/*
		* ONGLET DE FENÊTRE
		*
		* ---Contenu---
		* >Plein écran
		* >Séparer le canva en deux
		* >Rétablir le layout original
		*/
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Toggle Fullscreen")) {}
			if (ImGui::MenuItem("Split canva")) {}

			ImGui::Separator();

			if (ImGui::MenuItem("Reset Layout")) {}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
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

