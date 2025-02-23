// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

struct Shape {
    ImVec2 initPos;
    ImVec2 currPos;
    int type;
    int strokeWidth;
    ofFloatColor strokeColor;
    ofFloatColor fillColor;
};

/**
* \class DessinVectoriel
* \brief Gère le dessin vectoriel
*/
class DessinVectoriel {
public:
    /**
    * \brief Override la méthode setup
    */
    void setup();

    /**
    * \brief Override la méthode draw
    */
    void draw();

    /**
    * \brief Override la méthode drawGui
    */
    void drawGui();

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mousePressed(int x, int y, int button);

    /**
    * \brief This function gets called when the mouse is moving and the button 
    * is down. The button (left 0, center 1, right 2) variable can be used to 
    * test against left or right button drags. You also receive the x and y 
    * coordinates of the mouse.
    *
    * Called on the active window when the mouse is dragged, i.e. moved with a 
    * button pressed
    * \param int x position
    * \param int y position
    * \param int boutton
    *
    *
    * P.S. Override pour s'assurer que l'on prend cette méthode et non celle 
    * d'openframworks 'ofBaseApp::mouseDragged(ofMouseEventArgs &mouse)'
    */
    void mouseDragged(int x, int y, int button);

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mouseReleased(int x, int y, int button);

    // Initialisation
    void begin();
    bool isActive() const;
    void undo();
    void save();
    void exit();

    // Affichage des fenêtres
    void drawInit();
    void drawToolbar();

    // Fonctions liées aux dimensions des formes
    ImVec2 rectangleDims(const ImVec2& init, const ImVec2& pos);
    ImVec2 ellipseDims(const ImVec2& init, const ImVec2& pos);
    ImVec4 triangleDims(const ImVec2& init, const ImVec2& pos);

    // Construction des formes vectorielles
    Shape initShape() const;
    void drawShape(const Shape& s);
    void buildShape(const Shape& s, const bool& fill);

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
    int strokeWidth;
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
