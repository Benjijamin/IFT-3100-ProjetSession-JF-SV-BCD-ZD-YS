#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxImGui.h"
#include "SceneGraph.h"
#include "FreeFlightCamera.h"
#include "Editor.h"

class ModelEditor : public Editor {
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
    void switchToOrbitCamera();
    void switchToFreeFlightCamera();

    ofLight light;
    ofMaterial material;
    ofEasyCam orbitCam;
    FreeFlightCamera freeFlightCam;
    SceneGraph sceneGraph;

    bool isFreeFlightMode;
    bool shouldEnableMouseInput;
};
