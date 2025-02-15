#pragma once

#include "ofMain.h"


class DynamicCursor {
public:
    void setup();  // Initialiser les curseurs
    void update(int x, int y, ofRectangle& carre);  // Mise à jour du curseur

private:
    HCURSOR newCursor;  // Curseur personnalisé
};