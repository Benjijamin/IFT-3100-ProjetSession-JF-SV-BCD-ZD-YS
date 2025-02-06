#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxImGui.h"
#include "SceneGraph.h"
#include "FreeFlightCamera.h"

class ModelEditor {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void loadModel(const std::string& path);

private:
    void switchToOrbitCamera();
    void switchToFreeFlightCamera();

    ofLight light;
    ofMaterial material;
    ofEasyCam orbitCam;
    FreeFlightCamera freeFlightCam;
    SceneGraph sceneGraph;

    ofxImGui::Gui gui;

    bool isFreeFlightMode = false;
};
