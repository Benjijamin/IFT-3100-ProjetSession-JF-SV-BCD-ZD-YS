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
    void addEmptyNode(const std::string& name, SceneNode parent);
    void deleteNode(int index);

    void setSelectedIndex(int index);
    int getSelectedIndex() const;

    std::vector<int> getChildrenOf(int index);
    SceneNode getNode(int index) const;
    SceneNode getSelectedNode() const;
    SceneNode getRootNode() const;
private:
    void drawNodeGui(int index);

    std::string generateUniqueName(const std::string& baseName);

    int findParentIndex(int index);
    int findIndexOf(SceneNode node);
    void reassignChildrenToParent(int index, int parentIndex);
    void removeNodeFromGraph(int index);
    void updateChildrenIndices(int index);

    std::vector<SceneNode> nodes;
    std::unordered_map<int, std::vector<int>> childrenIndices;
    ofxGizmo gizmo;
    ofxGizmo::ofxGizmoType gizmoType;
    int selectedIndex;
};
