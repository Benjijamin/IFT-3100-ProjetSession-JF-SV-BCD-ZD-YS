// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once
#include "ofMain.h"

// Types de primitive 2D : point, ligne, rectangle, ellipse et triangle équilatéral
enum class TypePrimitive2D {none, point, ligne, rectangle, ellipse, triangle};

class DessinVectoriel
{
public:
    void setup();
    void draw();

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    TypePrimitive2D mode;
    float epaisseur_trait;

    int clicX;
    int clicY;
    bool clic;

    int curseurX;
    int curseurY;
};