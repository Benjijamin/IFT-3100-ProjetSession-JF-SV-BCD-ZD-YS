#include "SceneNode.h"

SceneNode::SceneNode(const std::string& name)
    : name(name) {}

void SceneNode::setModel(std::shared_ptr<ofxAssimpModelLoader> model) {
    this->model = model;
}

void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
    children.push_back(child);
    child->setParent(*this);
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    child->clearParent();
}

std::vector<std::shared_ptr<SceneNode>> SceneNode::getChildren() const {
    return children;
}

std::string SceneNode::getName() const {
    return name;
}

bool SceneNode::containsModel() const {
    return model != nullptr;
}

void SceneNode::customDraw() {
    if (model) {
        model->drawFaces();
    }
    else {
        ofDrawSphere(glm::vec3(0), 10);
    }
}

void SceneNode::draw() {
    ofPushMatrix();
    ofMultMatrix(getLocalTransformMatrix());
    customDraw();
    for (auto& child : children) {
        child->draw();
    }
    ofPopMatrix();
}
