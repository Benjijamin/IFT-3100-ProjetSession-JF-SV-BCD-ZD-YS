// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup() {

    // Taille et espacement des fenêtres
    windowGap = 30.0f;
    windowPos = ImVec2(windowGap / 2, windowGap);
    initSize = ImVec2(100.0f, 100.0f);
    toolbarSize = ImVec2(200.f, 600.0f);

    // Attributs des outils de dessin
    shapes = {};
    shapeType = ShapeType::triangle;
    strokeWidth = 16.0f;
    strokeColor = ofColor(0.0f);
    fillColor = ofColor(255.0f);
    bgColor = ofColor(128.0f);

    // Variables d'état
    active = false;
    mouseHeld = false;
    mouseGap = 2;
}

void DessinVectoriel::update() {

}

void DessinVectoriel::draw() {
    for (auto it = shapes.begin(); it != shapes.end(); ++it)
        buildShape(*it);
    if (mouseHeld) drawZone();
}

void DessinVectoriel::drawGui() {
    active ? drawToolbar() : drawInit();
}

void DessinVectoriel::exit() {
    shapes.clear();
    active = false;
}

void DessinVectoriel::mouseDragged(int x, int y, int button) {
    float clampX = min(max(mouseGap, x), ofGetWidth() - mouseGap);
    float clampY = min(max(mouseGap, y), ofGetHeight() - mouseGap);
    mousePos = ImVec2(clampX, clampY);
}

void DessinVectoriel::mousePressed(int x, int y, int button) {
    mouseHeld = true;
    mouseInit = ImVec2(x, y);
    mousePos = mouseInit;
}

void DessinVectoriel::mouseReleased(int x, int y, int button) {
    mouseHeld = false;
    addShape();
}

// TODO
void DessinVectoriel::save(const std::string& path) {

}

void DessinVectoriel::newDrawing() {
    active = true;
}

bool DessinVectoriel::isActive() {
    return active;
}

void DessinVectoriel::drawInit() {
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(initSize);

    ImGui::Begin("Dessin vectoriel");
    if (ImGui::Button("Nouveau dessin")) active = true;
    ImGui::End();
}

void DessinVectoriel::drawZone() {
    ofNoFill();
    ofSetLineWidth(strokeWidth);
    ofSetColor(255);
    ImVec2 dim = findRectDims(mouseInit, mousePos);
    ofDrawRectangle(mouseInit.x, mouseInit.y, dim.x, dim.y);
}

void DessinVectoriel::drawToolbar() {
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(toolbarSize);

    ImGui::Begin("Outils de dessin");

    ImGui::SliderInt("Épaisseur de trait", (int*) &strokeWidth, 0, 32);
    ImGui::ColorEdit3("Couleur de trait", (float*) &strokeColor);
    ImGui::ColorEdit3("Couleur de remplissage", (float*) &fillColor);
    ImGui::ColorEdit3("Couleur d'arrière-plan", (float*) &bgColor);

    if (ImGui::Button("Annuler"))
        exit();
    ImGui::End();
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

    // Largeur et hauteur du rectangle
    ImVec2 dim1 = findRectDims(s.initPos, s.currPos);
    ImVec2 dim2 = findEllipseDims(s.initPos, s.currPos);
    ImVec4 dim3 = findTriangleDims(s.initPos, s.currPos);
    ofSetCircleResolution(64);

    switch (s.type) {
    case ShapeType::point:
        ofDrawEllipse(s.initPos.x, s.initPos.y, s.strokeWidth, s.strokeWidth);
        break;
    case ShapeType::line:
        ofDrawLine(s.initPos.x, s.initPos.y, s.currPos.x, s.currPos.y);
        break;
    case ShapeType::rectangle:
        ofDrawRectangle(s.initPos.x, s.initPos.y, dim1.x, dim1.y);
        break;
    case ShapeType::ellipse:
        ofDrawEllipse(dim2.x, dim2.y, dim1.x, dim1.y);
        break;
    case ShapeType::triangle:
        ofDrawTriangle(s.initPos.x, s.initPos.y, dim3.x, dim3.y, dim3.z, dim3.w);
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

ImVec2 DessinVectoriel::findRectDims(const ImVec2& init, const ImVec2& pos) {
    return ImVec2(pos.x - init.x, pos.y - init.y);
}

ImVec2 DessinVectoriel::findEllipseDims(const ImVec2& init, const ImVec2& pos) {
    return ImVec2(init.x + (pos.x - init.x) / 2.0f, init.y + (pos.y - init.y) / 2.0f);
}

ImVec4 DessinVectoriel::findTriangleDims(const ImVec2& init, const ImVec2& pos) {
    return ImVec4((init.x + pos.x) / 2.0f, pos.y, pos.x, init.y);
}