#include "DynamicCursor.h"

void DynamicCursor::setup() {

    currentCursorType = DEFAULT;
    currentCursorHandle = getSystemCursor(DEFAULT);
    SetCursor(currentCursorHandle);
}

void DynamicCursor::update(int x, int y) {

    bool isOverAnyImGuiWindow = isMouseOverAnyImGuiWindow();

    if (isOverAnyImGuiWindow) {
        SetCursor(LoadCursor(NULL, IDC_HAND));
    }

    else {
        SetCursor(getSystemCursor(DEFAULT));
    }
}

void DynamicCursor::draw() {

}

void DynamicCursor::setCursorType(CursorType type) {
    if (currentCursorType != type) {
        currentCursorType = type;
        currentCursorHandle = getSystemCursor(type);
        SetCursor(currentCursorHandle);
    }
}

void DynamicCursor::resetCursor() {
    setCursorType(DEFAULT);
}

HCURSOR DynamicCursor::getSystemCursor(CursorType type) {
    switch (type) {
    case DEFAULT:
        return LoadCursor(NULL, IDC_ARROW);
    case HAND:
        return LoadCursor(NULL, IDC_HAND);
    case CROSSHAIR:
        return LoadCursor(NULL, IDC_CROSS);
    case RESIZE:
        return LoadCursor(NULL, IDC_SIZEALL);
    case WAIT:
        return LoadCursor(NULL, IDC_WAIT);
    default:
        return LoadCursor(NULL, IDC_ARROW);
    }
}

bool DynamicCursor::isMouseOverAnyImGuiWindow() {
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}