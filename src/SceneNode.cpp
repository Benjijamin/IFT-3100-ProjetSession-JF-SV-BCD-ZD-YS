#include "SceneNode.h"

SceneNode::SceneNode(const std::string& name)
    : name(name) {}

void SceneNode::setModel(std::shared_ptr<ofxAssimpModelLoader> model) {
    this->model = model;
}

void SceneNode::customDraw() {
    if (model) {
        model->drawFaces(); // Draw the model if it exists
    }
    else {
        ofDrawSphere(glm::vec3(0), 10); // Draw a default sphere otherwise
    }
}

std::string SceneNode::getName() const {
    return name;
}
