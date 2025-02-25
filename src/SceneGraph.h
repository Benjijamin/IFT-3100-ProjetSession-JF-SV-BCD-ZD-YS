#pragma once

#include <string>
#include <memory>
#include <functional>
#include "SceneNode.h"

class SceneGraph {
public:
    SceneGraph();

    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void addModelNode(const std::string& path);
    void addEmptyNode(const std::string& name);
    void unloadNodes(const std::string& path);
    void deleteNode(std::shared_ptr<SceneNode> node);
    void transferNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> newParent);
    void loadAsset(std::shared_ptr<SceneNode> node, std::string assetPath);

    std::shared_ptr<SceneNode> getRootNode() const;
    std::shared_ptr<SceneNode> getSelectedNode() const;
    void setSelectedNode(std::shared_ptr<SceneNode> node);
    void setSelectedNodeChangedCallback(std::function<void(std::shared_ptr<SceneNode>)> callback);

    std::string generateUniqueName(const std::string& baseName);
    bool editNodeName(std::shared_ptr<SceneNode> node, std::string& name);
private:
    void drawNodeGui(std::shared_ptr<SceneNode> node);

    void addNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent);
    void deleteNodesByBaseName(const std::string& baseName);

    bool isNameUnique(const std::shared_ptr<SceneNode>& node, const std::string& name);

    std::shared_ptr<SceneNode> rootNode;
    std::shared_ptr<SceneNode> selectedNode;

    std::function<void(std::shared_ptr<SceneNode>)> selectedNodeChangedCallback;
};
