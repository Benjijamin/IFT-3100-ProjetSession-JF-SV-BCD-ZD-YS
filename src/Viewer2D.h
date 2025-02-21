#pragma once

#include "ofMain.h"

class Viewer2D {
public:
    Viewer2D();

    void pan(float dx, float dy);
    void zoom(float scale, float mouseX, float mouseY);
    void adjustZoomAndPan(int contentWidth, int contentHeight);

    ofVec2f screenToPixelCoords(const ofVec2f& screenCoords) const;
    ofVec2f pixelToScreenCoords(const ofVec2f& pixelCoords) const;

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);

    ofVec2f getPanOffset() const { return panOffset; }
    float getZoomFactor() const { return zoomFactor; }
    bool isDraggingMouse() const { return isDragging; }
    ofVec2f getDragStartPos() const { return dragStartPos; }
    ofVec2f getDragEndPos() const { return dragEndPos; }

private:
    ofVec2f panOffset;
    float zoomFactor;
    bool isDragging;
    ofVec2f dragStartPos;
    ofVec2f dragEndPos;
};
