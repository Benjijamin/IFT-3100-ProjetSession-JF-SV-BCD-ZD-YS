#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "ColorPicker.h"
#include "Viewer2D.h"

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
    enum class Tool { PanZoom, CopyRegion, PasteRegion, Brush, Tint, Vibrance, Blur};

    void drawBrush(int startX, int startY, int endX, int endY);
    void copyRegion(int startX, int startY, int endX, int endY);
    void pasteRegion(int x, int y);
    void drawCopyRegion();
    void drawPasteRegion();
    bool isGuiHovered();
    bool isImageAllocated() const;
    bool isWithinBounds(int x, int y) const;

    Viewer2D viewer;
    ofImage* currentImage;
    ofVec2f imageDimensions;
    ofImage copiedRegion;
    ColorPicker colorPicker;
    ofColor drawColor;
    ofVec2f previousMousePos;
    int drawRadius;

    ofShader filterShader;

    ofColor tintColor;
    float saturationValue;
    float brightnessValue;
    float contrastValue;
    float blurValue;

    void setFilterValues();
    void applyFilter(ofVec2f panOffset);
    void bakeFilter();
    ofImage* originalImage = nullptr;

    void switchTool(Tool tool);
    Tool currentTool;
};