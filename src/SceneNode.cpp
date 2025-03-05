#include "SceneNode.h"
#include "Primitives.h"

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

void SceneNode::setName(std::string& newName)
{
    name = newName;
}

void SceneNode::setPrimitive(PrimitiveType newPrimitiveType) 
{
    primitiveType = newPrimitiveType; 

    switch (primitiveType)
    {
    case PrimitiveType::Sphere:
        break;
    case PrimitiveType::Tetrahedron:
        primitiveModel = Primitives::getTetrahedronPrimitive(1);
        break;
    case PrimitiveType::Cube:
        primitiveModel = Primitives::getCubePrimitive(1);
        break;
    case PrimitiveType::Cylinder:
        break;
    case PrimitiveType::Cone:
        break;
    default:
        break;
    }
}

bool SceneNode::containsModel() const {
    return model != nullptr;
}

void SceneNode::customDraw() {
    if (model) {
        model->drawFaces();
    }
    else if(primitiveType != PrimitiveType::None){
        switch (primitiveType)
        {
        case PrimitiveType::Sphere:
            ofDrawSphere(glm::vec3(0), 1);
            break;
        case PrimitiveType::Tetrahedron:
            primitiveModel->drawFaces();
            break;
        case PrimitiveType::Cube:
            primitiveModel->drawFaces();
            break;
        case PrimitiveType::Cylinder:
            ofDrawCylinder(glm::vec3(0), 1, 2);
            break;
        case PrimitiveType::Cone:
            ofDrawCone(glm::vec3(0), 1, 2);
            break;
        default:
            break;
        }
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

bool SceneNode::operator ==(const SceneNode& other) const 
{
    return (name == other.name);
}
