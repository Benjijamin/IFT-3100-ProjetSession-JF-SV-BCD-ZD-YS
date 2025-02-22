// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup() {
    mode = TypePrimitive2D::rectangle;
}

void DessinVectoriel::update() {

}

void DessinVectoriel::draw() {
}

void DessinVectoriel::drawGui() {
    if (nouveauDessin) {
        drawMainWindow();
        drawSettings();
    }
}

void DessinVectoriel::drawMainWindow() {
    ImGui::SetNextWindowPos(ImVec2(100, 100));
    ImGui::SetNextWindowSize(ImVec2(800, 600));

    ImGui::Begin("Canevas de dessin");
    hoverDessin = ImGui::IsWindowHovered();

    ofFill();
    ofDrawRectangle(100, 100, 800, 600);
    
    for (auto it = primitives.begin(); it != primitives.end(); ++it) {
        drawPrimitive(*it);
    }

    ImGui::End();
}

void DessinVectoriel::drawSettings() {
    ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - 100, 100));
    ImGui::Begin("Outils de dessin");
    ImGui::SliderFloat("Épaisseur de trait", &strokeWidth, 0.0f, 64.0f);
    if (ImGui::Button("Save")) {
        exit();
    }
    ImGui::End();
}

void DessinVectoriel::exit() {
    nouveauDessin = false;
}

void DessinVectoriel::mouseDragged(int x, int y, int button) {

}

void DessinVectoriel::mousePressed(int x, int y, int button) {
    if (hoverDessin) {
        clicX = x;
        clicY = y;
    }
}

void DessinVectoriel::mouseReleased(int x, int y, int button) {
    if (hoverDessin) {
        Primitive2D p;
        p.type = mode;
        p.pos1 = ImVec2(clicX, clicY);
        p.pos2 = ImVec2(x, y);
        p.strokeWidth = strokeWidth;
        p.strokeColor = strokeColor;
        p.fillColor = fillColor;
        primitives.push_back(p);
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

void DessinVectoriel::start() {
    nouveauDessin = true;
}

void DessinVectoriel::fillPrimitive(const Primitive2D& p) {
    ofFill();
    ofSetLineWidth(0);
    ofSetColor(p.fillColor);
}

void DessinVectoriel::strokePrimitive(const Primitive2D& p) {
    ofNoFill();
    ofSetColor(p.strokeColor);
    ofSetLineWidth(p.strokeWidth);
}

void DessinVectoriel::drawPrimitive(const Primitive2D& p) {
    switch (p.type) {
    case TypePrimitive2D::point:
        fillPrimitive(p);
        ofDrawEllipse(p.pos1.x, p.pos1.y, p.strokeWidth, p.strokeWidth);
        strokePrimitive(p);
        ofDrawEllipse(p.pos1.x, p.pos1.y, p.strokeWidth, p.strokeWidth);
        break;
    case TypePrimitive2D::ligne:
        strokePrimitive(p);
        ofDrawLine(p.pos1.x, p.pos1.y, p.pos2.x, p.pos2.y);
        break;
    case TypePrimitive2D::rectangle:
        fillPrimitive(p);
        ofDrawRectangle(p.pos1.x, p.pos1.y, p.pos2.x, p.pos2.y);
        strokePrimitive(p);
        ofDrawRectangle(p.pos1.x, p.pos1.y, p.pos2.x, p.pos2.y);
        break;
    case TypePrimitive2D::ellipse:
        fillPrimitive(p);
        ofDrawEllipse(p.pos1.x, p.pos1.y, p.pos2.x - p.pos1.x, p.pos2.y - p.pos1.y);
        strokePrimitive(p);
        ofDrawEllipse(p.pos1.x, p.pos1.y, p.pos2.x - p.pos1.x, p.pos2.y - p.pos1.y);
        break;
    case TypePrimitive2D::triangle:
        fillPrimitive(p);
        ofDrawTriangle(p.pos1.x, p.pos1.x, p.pos2.x / 2, p.pos2.y, p.pos2.x, p.pos1.y);
        strokePrimitive(p);
        ofDrawTriangle(p.pos1.x, p.pos1.x, p.pos2.x / 2, p.pos2.y, p.pos2.x, p.pos1.y);
        break;
    default:
        break;
    }
}