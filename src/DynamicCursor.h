#pragma once
#include "ofMain.h"
#include "ofxImGui.h"


class DynamicCursor {
public:
    enum CursorType {
        DEFAULT,    // Curseur par défaut (flèche)
        HAND,       // Curseur en forme de main
        CROSSHAIR,  // Curseur en forme de croix
        RESIZE,     // Curseur de redimensionnement
        WAIT        // Curseur d'attente
    };

    void setup();
    void update(int x, int y);
    void draw();

    void setCursorType(CursorType type);
    void resetCursor();

    bool isMouseOverAnyImGuiWindow();

private:
    CursorType currentCursorType;
    HCURSOR currentCursorHandle;

    HCURSOR getSystemCursor(CursorType type);
};