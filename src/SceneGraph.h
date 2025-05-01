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
    void drawVisibleNodes(const ofCamera& camera);
    void exit();

    void addEmptyNode(const std::string& name, std::shared_ptr<SceneNode> parent);
    void addPrimitiveNode(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent);
    void addModelNode(const std::string& path, std::shared_ptr<SceneNode> parent);
    void addLightNode(std::shared_ptr<ofLight> light, const std::string& name, std::shared_ptr<SceneNode> parent);
    void addSurfaceNode(const std::string& name, std::shared_ptr<SceneNode> parent);

    void unloadNodes(const std::string& path);
    void deleteNode(std::shared_ptr<SceneNode> node);
    void transferNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> newParent);

    std::shared_ptr<SceneNode> getRootNode() const;
    std::shared_ptr<SceneNode> getSelectedNode() const;
    void setSelectedNode(std::shared_ptr<SceneNode> node);

    bool editNodeName(std::shared_ptr<SceneNode> node, std::string& name);

private:
    void addNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent);
    void deleteNodesByBaseName(const std::string& baseName);

    std::string generateUniqueName(const std::string& baseName);
    bool isNameUnique(std::shared_ptr<SceneNode> node, const std::string& name);

    std::shared_ptr<SceneNode> rootNode;
    std::shared_ptr<SceneNode> selectedNode;
};
