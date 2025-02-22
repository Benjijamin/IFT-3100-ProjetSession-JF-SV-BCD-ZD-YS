// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"

// Types de primitive 2D : point, ligne, rectangle, ellipse et triangle équilatéral
enum class ShapeType { none, point, line, rectangle, ellipse, triangle };

struct Shape {
    ShapeType type;
    ImVec2 initPos;
    ImVec2 currPos;
    float strokeWidth;
    ofColor strokeColor;
    ofColor fillColor;
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
    * \brief Override la méthode update
    */
    void update();

    /**
    * \brief Override la méthode draw
    */
    void draw();

    /**
    * \brief Override la méthode drawGui
    */
    void drawGui();

    /**
    * \brief Override la méthode exit
    */
    void exit();

    /**
    * \brief This function gets called when the mouse is moving and the button is down. The button (left 0, center 1, right 2) variable can be used to test against left or right button drags. You also receive the x and y coordinates of the mouse.
    * 
    * Called on the active window when the mouse is dragged, i.e. moved with a button pressed
    * \param int x position
    * \param int y position
    * \param int boutton
    * 
    * 
    * P.S. Override pour s'assurer que l'on prend cette méthode et non celle d'openframworks 'ofBaseApp::mouseDragged(ofMouseEventArgs &mouse)'
    */
    void mouseDragged(int x, int y, int button);

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mousePressed(int x, int y, int button);

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mouseReleased(int x, int y, int button);

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int float
    */
    void mouseScrolled(int x, int y, float scrollX, float scrollY);

    /**
    * \brief (Explications)
    *
    * \param string path
    */
    void load(const std::string& path);

    void unload(const std::string& path);

    /**
    * \brief (Explications)
    *
    * \param string path
    */
    void save(const std::string& path);

    // Initialisation
    void newDrawing();
    bool isActive();

    // Affichage des fenêtres et de la zone de sélection
    void drawBlankWindow();
    void drawMainWindow();
    void drawZone();
    void drawToolbar();

    // Fonctions liées aux formes vectorielles
    void addShape();
    void drawShape(const Shape& s);
    void buildShape(const Shape& p);

private:

    // Taille et emplacement des fenêtres
    float windowGap;
    ImVec2 initWindowPos;
    ImVec2 blankWindowSize;
    ImVec2 mainWindowSize;
    ImVec2 toolbarSize;

    // Attributs des outils de dessin
    vector<Shape> shapes;
    ShapeType shapeType;
    float strokeWidth;
    ofColor strokeColor;
    ofColor fillColor;
    ofColor bgColor;

    // Contrôle de la souris
    bool mouseHeld;
    ImVec2 mouseInit;
    ImVec2 mousePos;

    // Variables d'état
    bool active;
    bool hovering;
};
