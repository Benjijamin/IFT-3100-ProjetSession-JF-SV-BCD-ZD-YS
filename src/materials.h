#pragma once

#include "ofMain.h"
#include <unordered_map>
#include <string>

/**
* @class Materials
* @brief Classe gérant les matériaux de base, création, importation, exportation et sauvegarde.
*/
class Materials {
public:
	void setup();
	void update();
	void draw();


	/**
	 * @brief Méthode créant un matériau par défaut.Ce dernier sert de place holder en attedant que l'utilisateur en import (ou créer) un.
	 */
	void defaultMaterial();

	/**
	 * @brief Méthode gérant la création d'un matériau à l'aide d'une interface
	 */
	void createMaterial(const std::string& name, const ofMaterial& material);

	/**
	 * @brief Import d'un matériau directement dans l'interface de matériau
	 */
	bool importMaterial(const std::string& filepath, const std::string& name);

	// Accesseurs
	ofMaterial& getMaterial(const std::string& name);
	ofMaterial& getDefaultMaterial();

private:
	std::unordered_map<std::string, ofMaterial> materials;
	ofMaterial default_material;

};

