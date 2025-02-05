#pragma once

#include "ofxGizmo.h"
#include "SceneNode.h"

class SceneGraph {
public:
    SceneGraph();

    void setup();
    void update();
    void draw(ofCamera& camera);
    void drawGui();
    void exit();

    void addModelNode(const std::string& name, std::shared_ptr<ofxAssimpModelLoader> model);
    void deleteNode(int index);

private:
    void drawNodeGui(int index);

    std::string generateUniqueName(const std::string& baseName);

    int findParentIndex(int index);
    void reassignChildrenToParent(int index, int parentIndex);
    void removeNodeFromGraph(int index);
    void updateChildrenIndices(int index);

    std::vector<SceneNode> nodes;
    std::unordered_map<int, std::vector<int>> childrenIndices;
    ofxGizmo gizmo;
    ofxGizmo::ofxGizmoType gizmoType;
    int selectedIndex;
};
