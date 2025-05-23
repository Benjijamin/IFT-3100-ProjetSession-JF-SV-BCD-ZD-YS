#pragma once

#include "ofxGizmo.h"
#include "ofxImGui.h"
#include "SceneNode.h"

class GizmoManager {
public:
    void setup();
    void update();
    void draw(ofCamera& camera);
    void drawGui();
    void exit();

    void setSelectedNode(std::shared_ptr<SceneNode> node);

    void onWindowResize(int w, int h);

private:
    ofxGizmo gizmo;
    ofxGizmo::ofxGizmoType gizmoType;
    std::shared_ptr<SceneNode> selectedNode;
};
