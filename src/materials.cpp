#include "materials.h"

// Initialisation
void Materials::setup() {
    defaultMaterial();
}

// Mise � jour (�ventuelle animation des mat�riaux)
void Materials::update() {}

// Dessin �ventuel (pr�visualisation, debug)
void Materials::draw() {}

// Cr�er un mat�riau par d�faut
void Materials::defaultMaterial() {
    default_material.setAmbientColor(ofColor(50, 50, 50));
    default_material.setDiffuseColor(ofColor(200, 200, 200));
    default_material.setSpecularColor(ofColor(255, 255, 255));
    default_material.setShininess(32.0f);

    materials["default"] = default_material;
}

// Cr�er un mat�riau personnalis�
void Materials::createMaterial(const std::string& name, const ofMaterial& material) {
    materials[name] = material;
}

// Importer un mat�riau � partir d'un fichier externe (impl�mentation basique)
bool Materials::importMaterial(const std::string& filepath, const std::string& name) {
    // exemple simple (� �tendre selon le format r�el)
    ofMaterial mat;
    // importer le mat�riau du fichier (� impl�menter)
    // pour l'instant, copier le default
    mat = default_material;

    materials[name] = mat;

    return true; // � changer selon succ�s
}

// Acc�s facile
ofMaterial& Materials::getMaterial(const std::string& name) {
    if (materials.find(name) != materials.end())
        return materials[name];

    // sinon retourne le default
    return default_material;
}

ofMaterial& Materials::getDefaultMaterial() {
    return default_material;
}
