// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup()
{
    // Taille et espacement des fenêtres
    windowGap = 12.0f;
    windowPos = ImVec2(windowGap / 3, windowGap * 2);
    windowSize = ImVec2(400.0f, 175.0f);

    // Attributs des outils de dessin
    shapes = {};
    types = { "Dot", "Line", "Rectangle", "Ellipse", "Triangle", "Dot trail" };
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
    active = false; // Renamed variable
    hovering = false;

    // Contrôle de la souris
    mouseHeld = false;
    mouseGap = 2;
    mouseInit = {};
    mousePos = {};

    // Courbe paramétrique
    curve.setup();
}

void DessinVectoriel::draw()
{
    if (!active) return;

    // Paramètres globaux
    ofSetBackgroundColor(bgColor);
    ofSetCircleResolution(circRes);

    ofPushStyle();

    // Tracer toutes les formes stockées en mémoire
    for (const auto& shape : shapes)
    {
        buildShape(shape, true);
    }

    // Tracer la zone de sélection
    if (mouseHeld && !hovering)
    {
        Shape s = initShape();
        s.strokeColor = zoneColor;
        buildShape(s, false);
    }

    // Courbe paramétrique
    curve.draw();

    ofPopStyle();
}

void DessinVectoriel::drawGui()
{
    if (!active || curve.isEditing()) return;

    // Drapeaux de la fenêtre
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));

    // Paramètres de la fenêtre
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);

    // Initialisation
    ImGui::Begin("Drawing tools", nullptr, flags);

    // Paramètres des outils de dessin
    ofxImGui::VectorCombo("Primitive type", &typeIndex, types);
    ImGui::SliderInt("Dot size", &dotSize, minWidth, maxWidth);
    ImGui::ColorEdit3("Stroke color", (float*)&strokeColor);
    ImGui::ColorEdit3("Fill color", (float*)&fillColor);
    ImGui::ColorEdit3("Background color", (float*)&bgColor);

    // Boutons d'action
    if (ImGui::Button("Undo")) undo();

    ImGui::SameLine();

    if (ImGui::Button("Quit")) exit();

    ImGui::PopStyleVar(3);

    // Fin
    ImGui::End();

    // Courbe paramétrique
    curve.drawGui();
}

void DessinVectoriel::exit()
{
    bgColor = prevBg;
    ofSetBackgroundColor(bgColor);

    active = false;
    shapes.clear();
    curve.exit();
}

void DessinVectoriel::begin()
{
    prevBg = ofGetBackgroundColor();
    active = true;
    shapes.clear();
    curve.begin();
}

void DessinVectoriel::mousePressed(int x, int y, int button)
{
    if (!curve.isEditing())
    {
        mouseHeld = true;
        mouseInit = ImVec2(x, y);
        mousePos = mouseInit;
        hovering = checkHover();
    }
}

void DessinVectoriel::mouseDragged(int x, int y, int button)
{
    if (!curve.isEditing())
    {
        // Restreindre la sélection à la fenêtre
        mousePos.x = std::clamp(x, mouseGap, ofGetWidth() - mouseGap);
        mousePos.y = std::clamp(y, mouseGap, ofGetHeight() - mouseGap);

        // Tracer la courbe si le mode Suite de points est sélectionné
        if (typeIndex == 5 && !hovering)
        {
            shapes.push_back(initShape());
        }
    }
}

void DessinVectoriel::mouseReleased(int x, int y, int button)
{
    if (curve.isEditing())
    {
        mouseHeld = false;

        if (!hovering)
        {
            shapes.push_back(initShape());
        }
    }
}

void DessinVectoriel::undo()
{
    if (shapes.empty()) return;

    bool isCurve = true;
    while (isCurve && shapes.size() > 1)
    {
        size_t i = shapes.size() - 1;
        isCurve = shapes[i].shapeType == 5; // Dot trail
        shapes.pop_back();
    }
}

Shape DessinVectoriel::initShape() const
{
    Shape s;
    s.dotSize = dotSize;
    s.shapeType = typeIndex;
    s.initPos = mouseInit;
    s.currPos = mousePos;
    s.strokeColor = strokeColor;
    s.fillColor = fillColor;
    return s;
}

void DessinVectoriel::buildShape(const Shape& s, const bool& fill)
{
    if (s.shapeType != 1 && fill)
    {
        ofFill();
        ofSetColor(s.fillColor);
    }
    else
    {
        ofNoFill();
        ofSetColor(s.strokeColor);
    }

    drawShape(s);
}

void DessinVectoriel::drawShape(const Shape& s)
{
    ImVec2 d1 = rectangleDims(s.initPos, s.currPos);
    ImVec2 d2 = ellipseDims(s.initPos, s.currPos);
    ImVec4 d3 = triangleDims(s.initPos, s.currPos);

    switch (s.shapeType)
    {
    case 1: // Line
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

bool DessinVectoriel::checkHover() const
{
    ImVec2 windowArea = windowPos + windowSize;
    bool withinBoundsX = mousePos.x >= windowPos.x && mousePos.x <= windowArea.x;
    bool withinBoundsY = mousePos.y >= windowPos.y && mousePos.y <= windowArea.y;
    return withinBoundsX && withinBoundsY;
}

// Helper Functions

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

// Méthodes inutilisées
void DessinVectoriel::update() {}
void DessinVectoriel::mouseScrolled(int x, int y, float scrollX, float scrollY) {}
void DessinVectoriel::load(const std::string& path) {}
void DessinVectoriel::save(const std::string& path) {}
