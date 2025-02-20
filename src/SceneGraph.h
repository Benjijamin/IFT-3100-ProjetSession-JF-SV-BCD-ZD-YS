#pragma once

#include "SceneNode.h"

class SceneGraph {
public:
    SceneGraph();

    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void addModelNode(const std::string& name, std::shared_ptr<ofxAssimpModelLoader> model);
    void deleteNode(int index);

    SceneNode& getNode(int index);
    int getSelectedIndex() const;

private:
    void drawNodeGui(int index);

    std::string generateUniqueName(const std::string& baseName);

    int findParentIndex(int index);
    void reassignChildrenToParent(int index, int parentIndex);
    void removeNodeFromGraph(int index);
    void updateChildrenIndices(int index);

    std::vector<SceneNode> nodes;
    std::unordered_map<int, std::vector<int>> childrenIndices;
    int selectedIndex;
};
