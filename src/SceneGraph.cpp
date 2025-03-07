#include "SceneGraph.h"
#include "AssetBrowser.h"

SceneGraph::SceneGraph() : selectedNode(nullptr) {}

void SceneGraph::setup() {
    rootNode = std::make_shared<SceneNode>("Root");
    setSelectedNode(rootNode);
}

void SceneGraph::update() {

}

void SceneGraph::draw() {
    if (rootNode) {
        rootNode->draw();
    }
}

void SceneGraph::drawVisibleNodes(const ofCamera& camera) {
    if (rootNode) {
        rootNode->drawVisibleNodes(camera);
    }
}

void SceneGraph::exit() {

}

void SceneGraph::addEmptyNode(const std::string& name, std::shared_ptr<SceneNode> parent) {
    auto newNode = std::make_shared<SceneNode>(generateUniqueName(name));

    if (parent) {
        parent->addChild(newNode);
    }
}

void SceneGraph::addPrimitiveNode(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent) {
    auto newNode = std::make_shared<SceneNode>(generateUniqueName(name));
    newNode->setPrimitive(primitiveType);

    if (parent) {
        parent->addChild(newNode);
    }
}

void SceneGraph::addModelNode(const std::string& path, std::shared_ptr<SceneNode> parent) {
    auto model = std::make_shared<ofxAssimpModelLoader>();
    model->load(path);
    model->setPosition(0, 0, 0);
    model->setRotation(0, 180, 1, 0, 0);

    std::string baseName = std::filesystem::path(path).stem().string();
    std::string uniqueName = generateUniqueName(baseName);

    auto newNode = std::make_shared<SceneNode>(uniqueName);
    newNode->setModel(model);

    if (parent) {
        parent->addChild(newNode);
    }
}

void SceneGraph::unloadNodes(const std::string& path) {
    std::string baseName = std::filesystem::path(path).stem().string();
    deleteNodesByBaseName(baseName);
}

std::shared_ptr<SceneNode> SceneGraph::getRootNode() const {
    return rootNode;
}

std::shared_ptr<SceneNode> SceneGraph::getSelectedNode() const {
    return selectedNode;
}

void SceneGraph::setSelectedNode(std::shared_ptr<SceneNode> node) {
    selectedNode = node;
}

void SceneGraph::addNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent) {
    parent->addChild(node);
}

void SceneGraph::deleteNode(std::shared_ptr<SceneNode> node) {
    if (node && node->getParent()) {
        auto parent = dynamic_cast<SceneNode*>(node->getParent());
        if (parent) {
            parent->removeChild(node);
        }
    }
}

void SceneGraph::transferNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> newParent) {
    deleteNode(node);
    addNode(node, newParent);
}

void SceneGraph::deleteNodesByBaseName(const std::string& baseName) {
    auto children = rootNode->getChildren();
    for (const auto& child : children) {
        if (child->getName().find(baseName) != std::string::npos) {
            deleteNode(child);
        }
    }
}

bool SceneGraph::isNameUnique(std::shared_ptr<SceneNode> node, const std::string& name) {
    if (node->getName() == name) {
        return false;
    }

    for (const auto& child : node->getChildren()) {
        if (!isNameUnique(child, name)) {
            return false;
        }
    }
}

std::string SceneGraph::generateUniqueName(const std::string& baseName) {
    std::string uniqueName = baseName;
    int counter = 1;

    while (!isNameUnique(rootNode, uniqueName)) {
        uniqueName = baseName + std::to_string(counter++);
    }

    return uniqueName;
}

bool SceneGraph::editNodeName(std::shared_ptr<SceneNode> node, std::string &name) {
    if (isNameUnique(node, name)) {
        node->setName(name);
        return true;
    }
    else {
        return false;
    }
}
