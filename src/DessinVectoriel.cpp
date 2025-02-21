// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup() {
    mode = TypePrimitive2D::point;
    epaisseur_trait = 2;
}

void DessinVectoriel::update() {

}

void DessinVectoriel::draw() {

}

void DessinVectoriel::drawGui() {

}

void DessinVectoriel::exit() {

}

void DessinVectoriel::mouseDragged(int x, int y, int button) {

}

void DessinVectoriel::mousePressed(int x, int y, int button) {
    clicX = x;
    clicY = y;
}

void DessinVectoriel::mouseReleased(int x, int y, int button) {
    curseurX = x;
    curseurY = y;

    switch (mode) {
    case TypePrimitive2D::point:
        ofDrawEllipse(clicX, clicY, epaisseur_trait, epaisseur_trait);
        break;
    default:
        break;
    }
}

void DessinVectoriel::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}

void DessinVectoriel::load(const std::string& path) {

}

void DessinVectoriel::unload(const std::string& path) {

}

void DessinVectoriel::save(const std::string& path) {

}
