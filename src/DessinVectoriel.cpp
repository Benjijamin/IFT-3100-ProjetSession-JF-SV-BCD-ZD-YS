// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup()
{
    // Taille et espacement des fenêtres
    windowGap = 30.0f;
    windowPos = ImVec2(windowGap / 2, windowGap);
    initSize = ImVec2(40.0f, 20.0f);
    toolbarSize = ImVec2(500.f, 150.0f);

    // Attributs des outils de dessin
    shapes = {};
    types = {"Point", "Ligne", "Rectangle", "Ellipse", "Triangle", "Suite de points"};
    typeIndex = 0;
    dotSize = 16;
    zoneColor = ofColor(255.0f);
    strokeColor = ofColor(0.0f);
    fillColor = ofColor(255.0f);
    bgColor = ofColor(60.0f);

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

void DessinVectoriel::update()
{

}

void DessinVectoriel::draw()
{
    // Paramètres globaux
    ofSetBackgroundColor(bgColor);
    ofSetCircleResolution(circRes);
    ofSetLineWidth(dotSize); // Debug

    // Tracer toutes les formes stockées en mémoire
    for (auto it = shapes.begin(); it != shapes.end(); ++it)
    {
        buildShape(*it, true);
    }

    // Tracer la zone de sélection
    if (mouseHeld && !hovering)
    {
        Shape s = initShape(false);
        s.strokeColor = zoneColor;
        buildShape(s, false);
    };

    // Cacher l'arrière de la barre d'outils
    buildShape(initShape(true), true);
}

void DessinVectoriel::drawGui()
{
    if (active) {
        drawToolbar();
    }
}

void DessinVectoriel::exit()
{
    shapes.clear();
    ofSetBackgroundColor(prevBg);
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
    // Restreindre la sélection à la fenêtre
    mousePos.x = min(max(mouseGap, x), ofGetWidth() - mouseGap);
    mousePos.y = min(max(mouseGap, y), ofGetHeight() - mouseGap);

    // Tracer la courbe si le mode Suite de points est sélectionné
    if (typeIndex == 5) shapes.push_back(initShape(false));
}

void DessinVectoriel::mouseReleased(int x, int y, int button)
{
    mouseHeld = false;
    if (!hovering)
    {
        Shape s = initShape(false);
        shapes.push_back(s);
    }
}

void DessinVectoriel::mouseScrolled(int x, int y, float scrollX, float scrollY)
{

}

void DessinVectoriel::load(const std::string& path)
{

}

void DessinVectoriel::save(const std::string& path)
{

}

void DessinVectoriel::begin()
{
    prevBg = ofGetBackgroundColor();
    active = true;
}

bool DessinVectoriel::isActive() const
{
    return active;
}

void DessinVectoriel::undo()
{
    shapes.pop_back();
    bool isCurve = true;
    while (isCurve && shapes.size() > 1)
    {
        size_t i = shapes.size() - 1;
        isCurve = shapes[i].shapeType == 5;
        shapes.pop_back();
    }
}

void DessinVectoriel::drawInit()
{
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(initSize);
    ImGui::Begin("Dessin vectoriel");
    if (ImGui::Button("Nouveau dessin")) begin();
    ImGui::End();
}

void DessinVectoriel::drawToolbar()
{
    // Drapeaux de la fenêtre
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;

    // Paramètres de la fenêtre
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(toolbarSize);
    ImGui::Begin("Outils de dessin", &active, flags);
    hovering = ImGui::IsWindowHovered();

    // Paramètres des outils de dessin
    ofxImGui::VectorCombo("Type de primitive", &typeIndex, types);
    ImGui::SliderInt("Taille de point", &dotSize, minWidth, maxWidth);
    ImGui::ColorEdit3("Couleur de trait", (float*) &strokeColor);
    ImGui::ColorEdit3("Couleur de remplissage", (float*) &fillColor);
    ImGui::ColorEdit3("Couleur d'arrière-plan", (float*) &bgColor);

    // Boutons d'action
    if (ImGui::Button("Annuler l'action")) undo();
    ImGui::SameLine();
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

Shape DessinVectoriel::initShape(const bool& cover) const
{
    // Initialiser la forme
    Shape s;
    s.dotSize = dotSize;

    // Si on trace la barre d'outils, tracer un polygone pour empêcher de tracer des formes
    if (cover)
    {
        s.shapeType = 2;
        s.initPos = ImVec2(0, 0);
        s.currPos = windowPos + toolbarSize;
        s.strokeColor = bgColor;
        s.fillColor = bgColor;
    }

    // Sinon, tracer la forme comme prévu
    else
    {
        s.shapeType = typeIndex;
        s.initPos = mouseInit;
        s.currPos = mousePos;
        s.strokeColor = strokeColor;
        s.fillColor = fillColor;
    }

    // Retour
    return s;
}

void DessinVectoriel::drawShape(const Shape& s)
{
    ImVec2 d1 = rectangleDims(s.initPos, s.currPos);
    ImVec2 d2 = ellipseDims(s.initPos, s.currPos);
    ImVec4 d3 = triangleDims(s.initPos, s.currPos);

    switch (s.shapeType)
    {
    case 1: // Ligne
        ofDrawLine(s.initPos.x, s.initPos.y, s.currPos.x, s.currPos.y);
        break;
    case 2: // Rectangle
        ofDrawRectangle(s.initPos.x, s.initPos.y, d1.x, d1.y);
        break;
    case 3: // Ellipse
        ofDrawEllipse(d2.x, d2.y, d1.x, d1.y);
        break;
    case 4: // Triangle
        ofDrawTriangle(s.initPos.x, s.initPos.y, d3.x, d3.y, d3.z, d3.w);
        break;
    default: // Point et suite de points
        ofDrawEllipse(s.currPos.x, s.currPos.y, s.dotSize, s.dotSize);
        break;
    }
}

void DessinVectoriel::buildShape(const Shape& s, const bool& fill)
{
    // Si la forme possède une surface (pas une ligne ni une zone de 
    // sélection), remplir cette surface
    if (s.shapeType != 1 && fill)
    {
        ofFill();
        ofSetColor(s.fillColor);
        drawShape(s);
    }

    // Tracer le contour de la forme
    ofNoFill();
    ofSetColor(s.strokeColor);
    drawShape(s);
}