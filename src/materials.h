#pragma once

#include "ofMain.h"
#include <unordered_map>
#include <string>

/**
* @class Materials
* @brief Classe g�rant les mat�riaux de base, cr�ation, importation, exportation et sauvegarde.
*/
class Materials {
public:
	void setup();
	void update();
	void draw();


	/**
	 * @brief M�thode cr�ant un mat�riau par d�faut.Ce dernier sert de place holder en attedant que l'utilisateur en import (ou cr�er) un.
	 */
	void defaultMaterial();

	/**
	 * @brief M�thode g�rant la cr�ation d'un mat�riau � l'aide d'une interface
	 */
	void createMaterial(const std::string& name, const ofMaterial& material);

	/**
	 * @brief Import d'un mat�riau directement dans l'interface de mat�riau
	 */
	bool importMaterial(const std::string& filepath, const std::string& name);

	// Accesseurs
	ofMaterial& getMaterial(const std::string& name);
	ofMaterial& getDefaultMaterial();

private:
	std::unordered_map<std::string, ofMaterial> materials;
	ofMaterial default_material;

};

