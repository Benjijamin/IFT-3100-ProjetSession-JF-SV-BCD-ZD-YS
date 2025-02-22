#pragma once

#include "ofxImGui.h"
#include "ScreenCapture.h"
#include "DessinVectoriel.h"

class MenuBar {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

private:
    void drawFileMenu();
    void drawSettingsMenu();
    void drawHelpMenu();

    ScreenCapture screenCapture;
    DessinVectoriel dessinVectoriel;
};
