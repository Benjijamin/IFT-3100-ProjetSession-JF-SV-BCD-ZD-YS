#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ImageEditor {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void mouseDragged(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void loadImage(const std::string& path);
    void saveImage(const std::string& path) const;

private:
    void panImage(float dx, float dy);
    void zoomImage(float scale, float mouseX, float mouseY);
    void adjustZoomAndPan();

    void drawCircle(int x, int y);
    void drawSquare(int x, int y);

    void copyRegion(int startX, int startY, int endX, int endY);
    void pasteRegion(int x, int y);

    bool isImageAllocated() const;
    bool isWithinBounds(int x, int y) const;

    ofVec2f screenToPixelCoords(const ofVec2f& screenCoords) const;
    ofVec2f pixelToScreenCoords(const ofVec2f& pixelCoords) const;

    ofImage* currentImage;

    ofVec2f panOffset;
    float zoomFactor;

    bool isDrawing;
    int drawRadius;
    ofColor drawColor;
    
    enum class Tool { PanZoom, Circle, Square, CopyRegion, PasteRegion };
    Tool currentTool;

    ofVec2f dragStartPos;
    ofVec2f dragEndPos;
    bool isDragging;

    ofImage copiedRegion;
};
