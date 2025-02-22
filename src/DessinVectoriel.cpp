// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup() {

    // Taille et espacement des fenêtres
    windowGap = 30.0f;
    initWindowPos = ImVec2(windowGap / 2, windowGap);
    blankWindowSize = ImVec2(100, 100);
    mainWindowSize = ImVec2(800, 600);
    toolbarSize = ImVec2(300, mainWindowSize.y);

    // Attributs des outils de dessin
    shapes = {};
    shapeType = ShapeType::rectangle;
    strokeWidth = 4.0f;
    strokeColor = ofColor(0);
    fillColor = ofColor(0, 255, 0);
    bgColor = ofColor(128);

    // Variables d'état
    active = false;
    hovering = false;
    mouseHeld = false;
}

void DessinVectoriel::update() {

}

void DessinVectoriel::draw() {

}

void DessinVectoriel::drawGui() {
    if (active) {
        drawMainWindow();
        drawToolbar();
    } else
        drawBlankWindow();
}

void DessinVectoriel::exit() {
    active = false;
}

void DessinVectoriel::mouseDragged(int x, int y, int button) {
    if (hovering)
        mousePos = ImVec2(x, y);
}

void DessinVectoriel::mousePressed(int x, int y, int button) {
    if (hovering) {
        mouseHeld = true;
        mouseInit = ImVec2(x, y);
        mousePos = mouseInit;
    }
}

void DessinVectoriel::mouseReleased(int x, int y, int button) {
    if (hovering) {
        mouseHeld = false;
        addShape();
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

void DessinVectoriel::newDrawing() {
    active = true;
}

bool DessinVectoriel::isActive() {
    return active;
}

void DessinVectoriel::drawBlankWindow() {
    ImGui::SetNextWindowPos(initWindowPos);
    ImGui::SetNextWindowSize(blankWindowSize);

    ImGui::Begin("Dessin vectoriel");
    if (ImGui::Button("Nouveau"))
        active = true;
    ImGui::End();
}

void DessinVectoriel::drawMainWindow() {
    ImGui::SetNextWindowPos(initWindowPos);
    ImGui::SetNextWindowSize(mainWindowSize);

    ImGui::Begin("Canevas de dessin");

    for (auto it = shapes.begin(); it != shapes.end(); ++it)
        buildShape(*it);

    hovering = ImGui::IsWindowHovered();

    if (hovering && mouseHeld)
        ImGui::Text("coucou"); // Debug
        drawZone();

    ImGui::End();
}

void DessinVectoriel::drawToolbar() {
    ImGui::SetNextWindowPos(ImVec2(mainWindowSize.x + windowGap, initWindowPos.y));
    ImGui::SetNextWindowSize(toolbarSize);

    ImGui::Begin("Outils de dessin");
    ImGui::SliderFloat("Épaisseur de trait", &strokeWidth, 0.0f, 32.0f);

    if (ImGui::Button("Annuler"))
        exit();
    ImGui::End();
}

void DessinVectoriel::drawZone() {
    ofNoFill();
    ofSetLineWidth(strokeWidth);
    ofSetColor(255);
    ofDrawRectangle(mouseInit.x, mouseInit.y, mousePos.x - mouseInit.x, mousePos.y - mouseInit.y);
}

void DessinVectoriel::addShape() {
    Shape shape;
    shape.type = shapeType;
    shape.initPos = mouseInit;
    shape.currPos = mousePos;
    shape.strokeWidth = strokeWidth;
    shape.strokeColor = strokeColor;
    shape.fillColor = fillColor;
    shapes.push_back(shape);
}

void DessinVectoriel::drawShape(const Shape& s) {
    ImVec2 dim = ImVec2(s.currPos.x - s.initPos.x, s.currPos.y - s.initPos.y);
    switch (s.type) {
    case ShapeType::point:
        ofDrawEllipse(s.initPos.x, s.initPos.y, s.strokeWidth, s.strokeWidth);
        break;
    case ShapeType::line:
        ofDrawLine(s.initPos.x, s.initPos.y, s.currPos.x, s.currPos.y);
        break;
    case ShapeType::rectangle:
        ofDrawRectangle(s.initPos.x, s.initPos.y, dim.x, dim.y);
        break;
    case ShapeType::ellipse:
        ofDrawEllipse(s.initPos.x + dim.x / 2.0f, s.initPos.y + dim.y / 2.0f , dim.x, dim.y);
        break;
    case ShapeType::triangle:
        ofDrawTriangle(s.initPos.x, s.initPos.x, s.currPos.x / 2, s.currPos.y, s.currPos.x, s.initPos.y);
        break;
    default:
        break;
    }
}

void DessinVectoriel::buildShape(const Shape& s) {
    if (s.type != ShapeType::line) {
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(s.fillColor);
        drawShape(s);
    }
    ofNoFill();
    ofSetLineWidth(s.strokeWidth);
    ofSetColor(s.strokeColor);
    drawShape(s);
}