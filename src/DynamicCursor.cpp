#include "DynamicCursor.h"

void DynamicCursor::setup() {
    // Initialiser le curseur par d�faut
    currentCursorType = DEFAULT;
    currentCursorHandle = getSystemCursor(DEFAULT);
    SetCursor(currentCursorHandle);  // Appliquer le curseur par d�faut
}

void DynamicCursor::update(int x, int y, bool isMouseOnMenuBar) {
    if (isMouseOnMenuBar) {
        SetCursor(getSystemCursor(HAND));   // Curseur main pour la barre de menus
    }
    
    else {
        SetCursor(getSystemCursor(DEFAULT)); // Curseur par d�faut
    }
}

void DynamicCursor::draw() {
    // Pas besoin de dessiner manuellement le curseur, car il est g�r� par Windows
}

void DynamicCursor::setCursorType(CursorType type) {
    if (currentCursorType != type) {
        currentCursorType = type;
        currentCursorHandle = getSystemCursor(type);
        SetCursor(currentCursorHandle);  // Appliquer le nouveau curseur
    }
}

void DynamicCursor::resetCursor() {
    setCursorType(DEFAULT);  // Revenir au curseur par d�faut
}

HCURSOR DynamicCursor::getSystemCursor(CursorType type) {
    switch (type) {
    case DEFAULT:
        return LoadCursor(NULL, IDC_ARROW);  // Curseur fl�che par d�faut
    case HAND:
        return LoadCursor(NULL, IDC_HAND);  // Curseur main (disponible � partir de Windows 2000)
    case CROSSHAIR:
        return LoadCursor(NULL, IDC_CROSS);  // Curseur croix
    case RESIZE:
        return LoadCursor(NULL, IDC_SIZEALL);  // Curseur de redimensionnement (fl�ches dans les 4 directions)
    case WAIT:
        return LoadCursor(NULL, IDC_WAIT);  // Curseur d'attente (sablier ou cercle de chargement)
    default:
        return LoadCursor(NULL, IDC_ARROW);  // Par d�faut, retourner le curseur fl�che
    }
}