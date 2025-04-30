#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "SceneGraph.h"
#include "GizmoManager.h"
#include "CameraManager.h"
#include "LightModal.h"
#include "MaterialModal.h"

class SceneEditor : public Editor {
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
    void drawSceneGraph();
    void drawSceneGraphNode(std::shared_ptr<SceneNode> node);
    void drawInfo(std::shared_ptr<SceneNode> node);
    void handleContextMenu(std::shared_ptr<SceneNode> node);
    void nodeDragDropBehaviour(std::shared_ptr<SceneNode> node);

    void newEmptyObject(const std::string& name, std::shared_ptr<SceneNode> parent);
    void newPrimitiveObject(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent);
    //void newSurfaceObject(const std::string& name, std::shared_ptr<SceneNode> parent);

    char* sceneName;
    SceneGraph sceneGraph;
    GizmoManager gizmoManager;
    CameraManager cameraManager;

    bool firstTime;
    bool justAddedNode;
    bool shouldEnableMouseInput;

    LightModal lightModal;
    MaterialModal materialModal;

    LightModal::LightType activeLightType;
    std::shared_ptr<ofLight> activeLight;
    std::shared_ptr<ofMaterial> activeMaterial;

    bool isLightPopupOpen;
    bool isMaterialPopupOpen;

    ofEasyCam camera;
};
