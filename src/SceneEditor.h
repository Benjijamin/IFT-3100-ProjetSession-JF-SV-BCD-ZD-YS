#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "SceneGraph.h"

class SceneEditor : public Editor
{
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
    void unload(const std::string& path);
    void save(const std::string& path) override;
private:
    char* sceneName;
    SceneGraph sceneGraph;

    bool justAddedNode;

    void drawSceneGraph();
    void drawSceneGraphNode(std::shared_ptr<SceneNode> node);
    void drawInfo(std::shared_ptr<SceneNode> node);
};