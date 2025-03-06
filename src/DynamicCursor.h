#pragma once
#include "ofMain.h"
#include <windows.h>  // Pour utiliser les curseurs système Windows

class DynamicCursor {
public:
    enum CursorType {
        DEFAULT,    // Curseur par défaut (flèche)
        HAND,       // Curseur en forme de main (pour les éléments interactifs)
        CROSSHAIR,  // Curseur en forme de croix (pour le dessin ou la sélection)
        RESIZE,     // Curseur de redimensionnement
        WAIT        // Curseur d'attente (par exemple, pendant un chargement)
    };

    void setup();
    void update(int x, int y,bool isMouseOnMenuBar);
    void draw();

    void setCursorType(CursorType type);  // Changer le type de curseur
    void resetCursor();                   // Réinitialiser le curseur à l'état par défaut

private:
    CursorType currentCursorType;  // Type de curseur actuel
    HCURSOR currentCursorHandle;   // Handle du curseur actuel

    // Méthode pour obtenir le handle du curseur système
    HCURSOR getSystemCursor(CursorType type);
};
