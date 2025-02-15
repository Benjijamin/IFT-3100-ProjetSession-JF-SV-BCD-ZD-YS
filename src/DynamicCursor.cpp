#include "DynamicCursor.h"


void DynamicCursor::setup() {
    newCursor = LoadCursorFromFile(L"icon.cur");  // Charger le curseur personnalisé
}

void DynamicCursor::update(int x, int y, ofRectangle& carre) {
    if (carre.inside(x, y)) {
        SetCursor(newCursor);  // Changer le curseur vers le personnalisé
    }
    else {
        SetCursor(LoadCursor(NULL, IDC_ARROW));  // Remettre le curseur par défaut
    }
}