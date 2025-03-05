// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"

struct Shape {
    ImVec2 initPos;
    ImVec2 currPos;
    int shapeType;
    int dotSize;
    ofFloatColor strokeColor;
    ofFloatColor fillColor;
};

/**
* \class DessinVectoriel
* \brief Gère le dessin vectoriel
*/
class DessinVectoriel : public Editor {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void drawGui() override;
    void exit() override;

    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;

    void load(const std::string& path) override;
    void save(const std::string& path) override;

    // Initialisation
    void begin();
    bool isActive() const;
    void undo();

    // Affichage des fenêtres
    void drawInit();
    void drawToolbar();

    // Fonctions liées aux dimensions des formes
    ImVec2 rectangleDims(const ImVec2& init, const ImVec2& pos);
    ImVec2 ellipseDims(const ImVec2& init, const ImVec2& pos);
    ImVec4 triangleDims(const ImVec2& init, const ImVec2& pos);

    // Construction des formes vectorielles
    Shape initShape(const bool& cover) const;
    void drawShape(const Shape& s);
    void buildShape(const Shape& s, const bool& fill);

    std::function<void()> onNewDrawing;

private:

    // Taille et emplacement des fenêtres
    float windowGap;
    ImVec2 windowPos;
    ImVec2 initSize;
    ImVec2 toolbarSize;

    // Attributs des outils de dessin
    vector<Shape> shapes;
    vector<string> types;
    int typeIndex;
    int dotSize;
    ofFloatColor zoneColor;
    ofFloatColor strokeColor;
    ofFloatColor fillColor;
    ofFloatColor bgColor;

    // Autres attributs
    int minWidth;
    int maxWidth;
    int circRes;
    ofColor prevBg;

    // Variables d'état
    bool active;
    bool hovering;

    // Contrôle de la souris
    bool mouseHeld;
    int mouseGap;
    ImVec2 mouseInit;
    ImVec2 mousePos;
};
