/**
 * \file Material.h
 * \brief Classe gérant la création de matériaux
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#pragma once

#include "ofMain.h"
#include <unordered_map>
#include <string>
#include <vector>

 /**
 * @class Materials
 * @brief Classe gérant les matériaux de base, création, importation, exportation et sauvegarde.
 */
class Materials {
public:
	/// Initialise le manager : crée le dossier data/materials, charge le default et les fichiers JSON existants
	void setup();

	/// Crée ou remplace un matériau et l'enregistre sur disque
	void create(const std::string& name, const ofMaterial& material);

	/// Retourne la liste des noms disponibles
	std::vector<std::string> list() const;

	/// Getter, Accède au matériau par nom, fallback sur "default"
	const ofMaterial& get(const std::string& name) const;

private:
	/// Construit le matériau "default"
	void makeDefaultMaterial();

	/// Sauvegarde un matériau nommé en JSON dans data/materials/name.json
	void saveMaterial(const std::string& name) const;

	/// Charge tous les .json de data/materials/ en matériaux
	void loadMaterials();

	std::unordered_map<std::string, ofMaterial> materials;
	ofMaterial default_material;
	std::string materialsPath;  // chemin absolu vers data/materials/
};

