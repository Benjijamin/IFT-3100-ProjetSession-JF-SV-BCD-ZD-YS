#include "SceneNode.h"

SceneNode::SceneNode(const std::string& name)
    : name(name) {}

void SceneNode::setModel(std::shared_ptr<ofxAssimpModelLoader> model) {
    this->model = model;
}

void SceneNode::customDraw() {
    if (model) {
        model->drawFaces();
    }
    else {
        ofDrawSphere(glm::vec3(0), 10);
    }
}

std::string SceneNode::getName() const {
    return name;
}
