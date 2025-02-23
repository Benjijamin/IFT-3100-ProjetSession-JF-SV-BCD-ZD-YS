// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup()
{
    // Taille et espacement des fenêtres
    windowGap = 30.0f;
    windowPos = ImVec2(windowGap / 2, windowGap);
    initSize = ImVec2(100.0f, 100.0f);
    toolbarSize = ImVec2(500.f, 200.0f);

    // Attributs des outils de dessin
    shapes = {};
    types = {"Point", "Ligne", "Rectangle", "Ellipse", "Triangle"};
    typeIndex = 0;
    strokeWidth = 16;
    zoneColor = ofColor(255.0f);
    strokeColor = ofColor(0.0f);
    fillColor = ofColor(255.0f);
    bgColor = ofColor(96.0f);

    // Autres attributs
    minWidth = 0;
    maxWidth = 32;
    circRes = 128;

    // Variables d'état
    active = false;
    hovering = false;

    // Contrôle de la souris
    mouseHeld = false;
    mouseGap = 2;
    mouseInit = {};
    mousePos = {};
}

void DessinVectoriel::draw()
{
    ofSetCircleResolution(circRes);
    for (auto it = shapes.begin(); it != shapes.end(); ++it)
        buildShape(*it, true);
    if (mouseHeld && !hovering) drawZone();
}

void DessinVectoriel::drawGui()
{
    active ? drawToolbar() : drawInit();
}

void DessinVectoriel::exit()
{
    shapes.clear();
    active = false;
}

void DessinVectoriel::mousePressed(int x, int y, int button)
{
    mouseHeld = true;
    mouseInit = ImVec2(x, y);
    mousePos = mouseInit;
}

void DessinVectoriel::mouseDragged(int x, int y, int button)
{
    mousePos.x = min(max(mouseGap, x), ofGetWidth() - mouseGap);
    mousePos.y = min(max(mouseGap, y), ofGetHeight() - mouseGap);
}

void DessinVectoriel::mouseReleased(int x, int y, int button)
{
    mouseHeld = false;
    if (!hovering)
    {
        Shape s = initShape();
        shapes.push_back(s);
    }
}

void DessinVectoriel::save(const std::string& path)
{

}

void DessinVectoriel::begin()
{
    active = true;
}

bool DessinVectoriel::isActive() const
{
    return active;
}

void DessinVectoriel::drawInit()
{
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(initSize);

    ImGui::Begin("Dessin vectoriel");
    if (ImGui::Button("Nouveau dessin")) active = true;
    ImGui::End();
}

void DessinVectoriel::drawToolbar()
{
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(toolbarSize);

    ImGui::Begin("Outils de dessin");
    hovering = ImGui::IsWindowHovered();

    ofxImGui::VectorCombo("Type de primitive", &typeIndex, types);
    ImGui::SliderInt("Épaisseur de trait", &strokeWidth, minWidth, maxWidth);

    ImGui::ColorEdit3("Couleur de trait", (float*) &strokeColor);
    ImGui::ColorEdit3("Couleur de remplissage", (float*) &fillColor);
    ImGui::ColorEdit3("Couleur d'arrière-plan", (float*) &bgColor);

    if (ImGui::Button("Annuler l'action") && shapes.size() > 2)
    {
        shapes.pop_back();
        shapes.pop_back();
    };

    if (ImGui::Button("Sauvegarder")) exit(); // TODO
    if (ImGui::Button("Quitter")) exit();

    ImGui::End();
}

ImVec2 DessinVectoriel::rectangleDims(const ImVec2& init, const ImVec2& pos)
{
    return ImVec2(pos.x - init.x, pos.y - init.y);
}

ImVec2 DessinVectoriel::ellipseDims(const ImVec2& init, const ImVec2& pos)
{
    return ImVec2((init.x + pos.x) / 2.0f, (init.y + pos.y) / 2.0f);
}

ImVec4 DessinVectoriel::triangleDims(const ImVec2& init, const ImVec2& pos)
{
    return ImVec4((init.x + pos.x) / 2.0f, pos.y, pos.x, init.y);
}

Shape DessinVectoriel::initShape() const
{
    Shape s;
    s.type = typeIndex;
    s.initPos = mouseInit;
    s.currPos = mousePos;
    s.strokeWidth = strokeWidth;
    s.strokeColor = strokeColor;
    s.fillColor = fillColor;
    return s;
}

void DessinVectoriel::drawShape(const Shape& s)
{
    ImVec2 d1 = rectangleDims(s.initPos, s.currPos);
    ImVec2 d2 = ellipseDims(s.initPos, s.currPos);
    ImVec4 d3 = triangleDims(s.initPos, s.currPos);

    switch (s.type)
    {
    case 0:
        ofDrawEllipse(s.currPos.x, s.currPos.y, s.strokeWidth, s.strokeWidth);
        break;
    case 1:
        ofDrawLine(s.initPos.x, s.initPos.y, s.currPos.x, s.currPos.y);
        break;
    case 2:
        ofDrawRectangle(s.initPos.x, s.initPos.y, d1.x, d1.y);
        break;
    case 3:
        ofDrawEllipse(d2.x, d2.y, d1.x, d1.y);
        break;
    case 4:
        ofDrawTriangle(s.initPos.x, s.initPos.y, d3.x, d3.y, d3.z, d3.w);
        break;
    }
}

void DessinVectoriel::buildShape(const Shape& s, const bool& fill)
{
    if (s.type != 1 && fill)
    {
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

void DessinVectoriel::drawZone()
{
    Shape s = initShape();
    s.strokeColor = zoneColor;
    buildShape(s, false);
}