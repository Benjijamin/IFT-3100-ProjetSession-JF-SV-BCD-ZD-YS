#include "SceneNode.h"

SceneNode::SceneNode(const std::string& name)
    : name(name), primitiveType(PrimitiveType::None) {
    boundingBox.initToInfinity();
}

SceneNode::~SceneNode() {
    if (light) {
        light->disable();
    }
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

void SceneNode::setName(const std::string& newName) {
    name = newName;
}

void SceneNode::setPrimitive(PrimitiveType newPrimitiveType) {
    primitiveType = newPrimitiveType;

    switch (primitiveType) {
    case PrimitiveType::Sphere:
        break;
    case PrimitiveType::Tetrahedron:
        primitiveModel = Primitives::getTetrahedronPrimitive(100.0f);
        break;
    case PrimitiveType::Cube:
        primitiveModel = Primitives::getCubePrimitive(100.0f);
        break;
    case PrimitiveType::Cylinder:
        break;
    case PrimitiveType::Cone:
        break;
    default:
        break;
    }

    setDefaultMaterial();
}

void SceneNode::setModel(std::shared_ptr<ofxAssimpModelLoader> newModel) {
    model = newModel;
    boundingBox.compute(model);

    model->disableMaterials();

    setDefaultMaterial();
}

bool SceneNode::hasModel() const {
    return model != nullptr;
}

void SceneNode::setDefaultMaterial() {
    material = std::make_shared<ofMaterial>();

    material->setDiffuseColor(ofFloatColor(0.4f, 0.4f, 0.4f, 1.0f));
    material->setAmbientColor(ofFloatColor(0.3f, 0.3f, 0.3f, 1.0f));
    material->setSpecularColor(ofFloatColor(0.6f, 0.6f, 0.6f, 1.0f));
    material->setEmissiveColor(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    material->setShininess(32);
}

void SceneNode::setLightMaterial() {
    material = std::make_shared<ofMaterial>();

    material->setAmbientColor(ofFloatColor(0.2f, 0.2f, 0.2f, 1.0f));
    material->setDiffuseColor(ofFloatColor(0.8f, 0.8f, 0.8f, 1.0f));
    material->setSpecularColor(ofFloatColor(0.9f, 0.9f, 0.9f, 1.0f));
    material->setEmissiveColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
    material->setShininess(100);
}

bool SceneNode::hasMaterial() const {
    return material != nullptr;
}

std::shared_ptr<ofMaterial> SceneNode::getMaterial() const {
    return material;
}

void SceneNode::setLight(std::shared_ptr<ofLight> newLight) {
    this->light = newLight;

    setLightMaterial();
}

bool SceneNode::hasLight() const {
    return light != nullptr;
}

std::shared_ptr<ofLight> SceneNode::getLight() const {
    return light;
}

void SceneNode::setTexture(const std::string& path)
{
    textureImage.load(path);
    texture = textureImage.getTextureReference();
}

void SceneNode::setSurface(std::shared_ptr<ofMesh> surfaceMesh) 
{
    surface = surfaceMesh;

    setDefaultMaterial();
}

void SceneNode::updateSurface() 
{
    auto parent = dynamic_cast<SceneNode*>(getParent());

    if (surface) 
    {
        surface = Primitives::getBezierSurface(getControlPoints(), 10);
    }
    else if(parent->surface)
    {
        parent->surface = Primitives::getBezierSurface(parent->getControlPoints(), 10);
    }
}

bool SceneNode::isSurfaceControl() const
{
    if (getParent()) 
    {
        auto parent = dynamic_cast<SceneNode*>(getParent());
        if (parent->surface) 
        {
            return true;
        }
    }

    return false;
}

std::vector<glm::vec3> SceneNode::getControlPoints() const
{
    std::vector<glm::vec3> controlPoints;
    auto children = getChildren();
    controlPoints.resize(children.size());

    for (size_t i = 0; i < children.size(); i++)
    {
        controlPoints[i] = children[i]->getPosition();
    }

    return controlPoints;
}

void SceneNode::customDraw() {
    if (texture.isAllocated())
    {
        texture.bind();
    }

    if (light) {
        drawLight();
    }
    else if (model) {
        model->drawFaces();
    }
    else if (surface) {
        drawSurface();
    }
    else {
        drawPrimitive();
    }
}

void SceneNode::drawLight() {
    ofPushMatrix();

    if (light->getIsPointLight()) {
        ofDrawSphere(glm::vec3(0, 0, 0), 10.0f);
        ofDrawAxis(20);
    }
    else if (light->getIsSpotlight()) {
        float coneHeight = (sin(ofDegToRad(light->getSpotlightCutOff())) * 30.0f) + 1.0f;
        float coneRadius = (cos(ofDegToRad(light->getSpotlightCutOff())) * 30.0f) + 8.0f;
        ofRotateDeg(270, 1, 0, 0);
        ofDrawCone(0, -(coneHeight * 0.5f), 0, coneRadius, coneHeight);
    }
    else if (light->getIsDirectional()) {
        ofDrawBox(glm::vec3(0, 0, 0), 10.0f);
        ofDrawArrow(glm::vec3(0, 0, 0), glm::vec3(0, 0, -40), 10.0f);
    }
    else if (light->getIsAreaLight()) {
        ofDrawPlane(glm::vec3(0, 0, 0), 30.0f, 30.0f);
        ofDrawArrow(glm::vec3(0, 0, 0), glm::vec3(0, 0, -30), 10.0f);
    }
    else {
        ofDrawBox(glm::vec3(0, 0, 0), 10.0f);
        ofDrawAxis(20);
    }

    ofPopMatrix();
}

void SceneNode::drawPrimitive() {
    switch (primitiveType) {
    case PrimitiveType::Sphere:
        ofDrawSphere(glm::vec3(0.0f), 100.0f);
        break;
    case PrimitiveType::Tetrahedron:
    case PrimitiveType::Cube:
        if (primitiveModel) {
            primitiveModel->drawFaces();
        }
        break;
    case PrimitiveType::Cylinder:
        ofDrawCylinder(glm::vec3(0.0f), 100.0f, 200.0f);
        break;
    case PrimitiveType::Cone:
        ofDrawCone(glm::vec3(0.0f), 100.0f, 200.0f);
        break;
    default:
        break;
    }

    texture.unbind();
}

void SceneNode::drawSurface() 
{
    surface->drawFaces();
    for (auto& control : children) {
        ofDrawSphere(control->getPosition(), 1);
    }
}
void SceneNode::draw() {
    ofPushMatrix();
    ofMultMatrix(getLocalTransformMatrix());

    if (light) {
        light->setPosition(getGlobalTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        glm::mat4 rotationMatrix = getGlobalTransformMatrix();
        glm::quat orientationQuat = glm::quat_cast(rotationMatrix);
        glm::vec3 eulerAngles = glm::eulerAngles(orientationQuat);

        light->setOrientation(ofVec3f(ofRadToDeg(eulerAngles.x), ofRadToDeg(eulerAngles.y), ofRadToDeg(eulerAngles.z)));
    }

    if (material) {
        material->begin();
    }

    customDraw();

    if (material) {
        material->end();
    }

    for (auto& child : children) {
        child->draw();
    }

    ofPopMatrix();
}

void SceneNode::drawVisibleNodes(const ofCamera& camera) {
    ofPushMatrix();
    ofMultMatrix(getLocalTransformMatrix());

    if (boundingBox.isVisible(camera, getGlobalTransformMatrix())) {
        ofSetColor(ofColor::green);
    }
    else {
        ofSetColor(ofColor::red);
    }

    boundingBox.draw();

    for (auto& child : children) {
        child->drawVisibleNodes(camera);
    }

    ofPopMatrix();
}

ofBoxPrimitive SceneNode::getBoundingBox() const {
    return boundingBox.getBox();
}

bool SceneNode::operator==(const SceneNode& other) const {
    return name == other.name;
}

