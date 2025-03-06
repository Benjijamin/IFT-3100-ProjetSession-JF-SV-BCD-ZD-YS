#pragma once
#include "ofMain.h"
#include <windows.h>  // Pour utiliser les curseurs syst�me Windows

class DynamicCursor {
public:
    enum CursorType {
        DEFAULT,    // Curseur par d�faut (fl�che)
        HAND,       // Curseur en forme de main (pour les �l�ments interactifs)
        CROSSHAIR,  // Curseur en forme de croix (pour le dessin ou la s�lection)
        RESIZE,     // Curseur de redimensionnement
        WAIT        // Curseur d'attente (par exemple, pendant un chargement)
    };

    void setup();
    void update(int x, int y,bool isMouseOnMenuBar);
    void draw();

    void setCursorType(CursorType type);  // Changer le type de curseur
    void resetCursor();                   // R�initialiser le curseur � l'�tat par d�faut

private:
    CursorType currentCursorType;  // Type de curseur actuel
    HCURSOR currentCursorHandle;   // Handle du curseur actuel

    // M�thode pour obtenir le handle du curseur syst�me
    HCURSOR getSystemCursor(CursorType type);
};
