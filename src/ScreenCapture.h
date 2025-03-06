#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include <windows.h>
#include "DynamicCursor.h"

class ScreenCapture {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void captureScreenshot();
    void saveScreenshot();

    void mouseDragged(int x, int y, int button);

private:
    void createScreenshotTexture();
    void deleteScreenshotTexture();

    ofImage screenshotImg;
    GLuint screenshotTexture;

    bool showScreenshotModal;
    bool screenshotTaken;
};
