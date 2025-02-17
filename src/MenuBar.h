#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include "imageEditor.h"

/*
* \brief Classe qui gère la barre de menu.
*/
class MenuBar 
{
	public:
		/*
		* \brief Méthode de la librairie openframeworks
		*/
		void drawGui();

		/*
		* \brief Gestion de la sauvegarde des projets
		* \param filePath --> Type .json
		*/
		void saveProject(const std::string& filePath);

		/*
		* \brief Gestion du chargement de projets existants
		* \param filePath --> Type .json
		*/
		void openProject(const std::string& filePath);

		/*
		* \brief Instance de 'ImageEditor' pour accéder à ses méthodes
		*/
		ImageEditor imageEditor;
};