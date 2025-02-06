// DessinVectoriel.cpp
// Implémentation de la création de primitives 2D

#include "DessinVectoriel.h"

void DessinVectoriel::setup()
{
    mode = TypePrimitive2D::point;
    epaisseur_trait = 2;
}

void DessinVectoriel::draw()
{
}

void DessinVectoriel::mousePressed(int x, int y, int button) {
    clicX = x;
    clicY = y;
}

void DessinVectoriel::mouseReleased(int x, int y, int button) {
    curseurX = x;
    curseurY = y;
    switch (mode)
    {
        case TypePrimitive2D::point:
            ofDrawEllipse(clicX, clicY, epaisseur_trait);
            break;
        default:
            break;
    }
}