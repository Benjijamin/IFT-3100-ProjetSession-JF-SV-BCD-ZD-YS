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
    void createScreenshotTexture();
    void deleteScreenshotTexture();
    void showModal();
    void hideModal();

private:
    bool showScreenshotModalFlag = false;
    bool screenshotTaken = false;

    ofImage screenshotImg;
    GLuint screenshotTexture;
};
