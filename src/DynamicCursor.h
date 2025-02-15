#pragma once

#include "ofMain.h"


class DynamicCursor {
public:
    void setup();  // Initialiser les curseurs
    void update(int x, int y, ofRectangle& carre);  // Mise � jour du curseur

private:
    HCURSOR newCursor;  // Curseur personnalis�
};