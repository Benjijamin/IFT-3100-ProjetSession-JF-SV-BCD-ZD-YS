#include "materials.h"

// Initialisation
void Materials::setup() {
    defaultMaterial();
}

// Mise à jour (éventuelle animation des matériaux)
void Materials::update() {}

// Dessin éventuel (prévisualisation, debug)
void Materials::draw() {}

// Créer un matériau par défaut
void Materials::defaultMaterial() {
    default_material.setAmbientColor(ofColor(50, 50, 50));
    default_material.setDiffuseColor(ofColor(200, 200, 200));
    default_material.setSpecularColor(ofColor(255, 255, 255));
    default_material.setShininess(32.0f);

    materials["default"] = default_material;
}

// Créer un matériau personnalisé
void Materials::createMaterial(const std::string& name, const ofMaterial& material) {
    materials[name] = material;
}

// Importer un matériau à partir d'un fichier externe (implémentation basique)
bool Materials::importMaterial(const std::string& filepath, const std::string& name) {
    // exemple simple (à étendre selon le format réel)
    ofMaterial mat;
    // importer le matériau du fichier (à implémenter)
    // pour l'instant, copier le default
    mat = default_material;

    materials[name] = mat;

    return true; // à changer selon succès
}

// Accès facile
ofMaterial& Materials::getMaterial(const std::string& name) {
    if (materials.find(name) != materials.end())
        return materials[name];

    // sinon retourne le default
    return default_material;
}

ofMaterial& Materials::getDefaultMaterial() {
    return default_material;
}
