#include "Viewer2D.h"

Viewer2D::Viewer2D() : zoomFactor(1.0f), isDragging(false) {
    panOffset.set(0, 0);
    zoomFactor = 1.0f;
    isDragging = false;
}

void Viewer2D::pan(float dx, float dy) {
    panOffset.x += dx;
    panOffset.y += dy;
}

void Viewer2D::zoom(float scale, float mouseX, float mouseY) {
    ofVec2f prevMousePos = (ofVec2f(mouseX, mouseY) - panOffset) / zoomFactor;

    zoomFactor *= scale;
    if (zoomFactor < 0.1f) {
        zoomFactor = 0.1f;
    }
    else if (zoomFactor > 10.0f) {
        zoomFactor = 10.0f;
    }

    ofVec2f newMousePos = (ofVec2f(mouseX, mouseY) - panOffset) / zoomFactor;
    ofVec2f panAdjust = (newMousePos - prevMousePos) * zoomFactor;
    panOffset += panAdjust;
}

void Viewer2D::adjustZoomAndPan(int contentWidth, int contentHeight) {
    int screenWidth = ofGetWidth();
    int screenHeight = ofGetHeight();

    float targetWidth = screenWidth * 0.7f;
    zoomFactor = targetWidth / contentWidth;

    panOffset.set((screenWidth - contentWidth * zoomFactor) / 2, (screenHeight - contentHeight * zoomFactor) / 2);
}

ofVec2f Viewer2D::screenToPixelCoords(const ofVec2f& screenCoords) const {
    return (screenCoords - panOffset) / zoomFactor;
}

ofVec2f Viewer2D::pixelToScreenCoords(const ofVec2f& pixelCoords) const {
    return pixelCoords * zoomFactor + panOffset;
}

void Viewer2D::mouseDragged(int x, int y, int button) {
    if (isDragging) {
        pan(x - dragStartPos.x, y - dragStartPos.y);
        dragStartPos = ofVec2f(x, y);
    }
}

void Viewer2D::mousePressed(int x, int y, int button) {
    dragStartPos.set(x, y);
    isDragging = true;
}

void Viewer2D::mouseReleased(int x, int y, int button) {
    dragEndPos.set(x, y);
    isDragging = false;
}

void Viewer2D::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    zoom(1.0f + scrollY * 0.1f, x, y);
}
