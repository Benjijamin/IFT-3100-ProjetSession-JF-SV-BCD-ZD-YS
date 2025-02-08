// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once

#include "ofMain.h"
#include "Editor.h"

// Types de primitive 2D : point, ligne, rectangle, ellipse et triangle équilatéral
enum class TypePrimitive2D { none, point, ligne, rectangle, ellipse, triangle };

/**
* \class DessinVectoriel
* \brief Gère le dessin vectoriel
*/
class DessinVectoriel : public Editor {
public:
    /**
    * \brief Override la méthode setup
    */
    void setup() override;

    /**
    * \brief Override la méthode update
    */
    void update() override;

    /**
    * \brief Override la méthode draw
    */
    void draw() override;

    /**
    * \brief Override la méthode drawGui
    */
    void drawGui() override;

    /**
    * \brief Override la méthode exit
    */
    void exit() override;

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
    void mouseDragged(int x, int y, int button) override;

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mousePressed(int x, int y, int button) override;

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int boutton
    */
    void mouseReleased(int x, int y, int button) override;

    /**
    * \brief (Explications)
    *
    * \param int x position
    * \param int y position
    * \param int float
    */
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;

    /**
    * \brief (Explications)
    *
    * \param string path
    */
    void load(const std::string& path) override;

    /**
    * \brief (Explications)
    *
    * \param string path
    */
    void save(const std::string& path) override;

private:
    TypePrimitive2D mode;
    float epaisseur_trait;

    int clicX;
    int clicY;
    bool clic;

    int curseurX;
    int curseurY;
};
