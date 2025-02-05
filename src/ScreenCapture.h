#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ScreenCapture {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void captureScreenshot();
    void saveScreenshot();

private:
    void createScreenshotTexture();
    void deleteScreenshotTexture();

    ofImage screenshotImg;
    GLuint screenshotTexture;

    bool showScreenshotModal;
    bool screenshotTaken;
};
