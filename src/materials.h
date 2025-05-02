#pragma once

#include "ofMain.h"
#include <unordered_map>
#include <string>
#include <vector>

/**
* @class Materials
* @brief Classe g�rant les mat�riaux de base, cr�ation, importation, exportation et sauvegarde.
*/
class Materials {
public:
	/// Initialise le manager : cr�e le dossier data/materials, charge le default et les fichiers JSON existants
	void setup();

	/// Cr�e ou remplace un mat�riau et l'enregistre sur disque
	void create(const std::string& name, const ofMaterial& material);

	/// Retourne la liste des noms disponibles
	std::vector<std::string> list() const;

	/// Getter, Acc�de au mat�riau par nom, fallback sur "default"
	const ofMaterial& get(const std::string& name) const;

private:
	/// Construit le mat�riau "default"
	void makeDefaultMaterial();

	/// Sauvegarde un mat�riau nomm� en JSON dans data/materials/name.json
	void saveMaterial(const std::string& name) const;

	/// Charge tous les .json de data/materials/ en mat�riaux
	void loadMaterials();

	std::unordered_map<std::string, ofMaterial> materials;
	ofMaterial default_material;
	std::string materialsPath;  // chemin absolu vers data/materials/
};

