#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "ColorPicker.h"

class ImageEditor : public Editor {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void drawGui() override;
    void exit() override;

    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;

    void load(const std::string& path) override;
    void save(const std::string& path) override;

private:
    void panImage(float dx, float dy);
    void zoomImage(float scale, float mouseX, float mouseY);
    void adjustZoomAndPan();

    void drawCircle(int x, int y);
    void drawSquare(int x, int y);

    void copyRegion(int startX, int startY, int endX, int endY);
    void pasteRegion(int x, int y);
    void drawCopyRegion();
    void drawPasteRegion();

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

    //ColorPicker
    ColorPicker colorPicker;

    ofImage colorPickerIcon;
    ofTexture colorPickerIconTex;
};
