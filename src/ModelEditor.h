#pragma once

#include "ofMain.h"
#include "ofxGizmo.h"
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
    void unload(const std::string& path) override;
    void save(const std::string& path) override;

private:
    void drawGizmoControls();
    void drawCameraControls();
    void drawFreeFlightParameters();
    void drawPerspectiveParameters();
    void drawOrthographicParameters();

    void switchToOrbitCamera();
    void switchToFreeFlightCamera();
    void updateGizmo();
    ofCamera* getActiveCamera();

    ofLight light;
    ofMaterial material;

    ofEasyCam orbitCam;
    FreeFlightCamera freeFlightCam;
    bool isFreeFlightMode;
    bool shouldEnableMouseInput;

    SceneGraph sceneGraph;
    ofxGizmo gizmo;
    ofxGizmo::ofxGizmoType gizmoType;
};
