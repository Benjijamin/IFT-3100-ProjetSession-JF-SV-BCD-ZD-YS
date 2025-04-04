#include "SceneNode.h"
#include "Primitives.h"

SceneNode::SceneNode(const std::string& name)
    : name(name), primitiveType(PrimitiveType::None) {
    initAABBToInfinity();
}

void SceneNode::setModel(std::shared_ptr<ofxAssimpModelLoader> model) {
    this->model = model;
    computeAABB(model);
}

void SceneNode::setTexture(const std::string& path) 
{
    textureImage.load(path);
    texture = textureImage.getTextureReference();
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

void SceneNode::setName(std::string& newName) {
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
}

bool SceneNode::containsModel() const {
    return model != nullptr;
}

void SceneNode::customDraw() {
    if (texture.isAllocated()) 
    {
        texture.bind();
    }

    if (model) {
        model->drawFaces();
    }
    else {
        switch (primitiveType) {
        case PrimitiveType::Sphere:
            ofDrawSphere(glm::vec3(0.0f), 100.0f);
            break;
        case PrimitiveType::Tetrahedron:
            primitiveModel->drawFaces();
            break;
        case PrimitiveType::Cube:
            primitiveModel->drawFaces();
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
    }

    texture.unbind();
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

void SceneNode::drawVisibleNodes(const ofCamera& camera) {
    ofPushMatrix();
    ofMultMatrix(getLocalTransformMatrix());
    
    customDraw();

    if (isAABBVisible(camera)) {
        ofSetColor(ofColor::green);
    }
    else {
        ofSetColor(ofColor::red);
    }

    drawAABBBox();

    for (auto& child : children) {
        child->drawVisibleNodes(camera);
    }
    
    ofPopMatrix();
}

ofBoxPrimitive SceneNode::getAABB() const {
    return aabb;
}

void SceneNode::initAABBToInfinity() {
    ofVec3f minBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    ofVec3f maxBounds(FLT_MAX, FLT_MAX, FLT_MAX);

    aabb.setPosition((minBounds + maxBounds) * 0.5f);
    aabb.setWidth(maxBounds.x - minBounds.x);
    aabb.setHeight(maxBounds.y - minBounds.y);
    aabb.setDepth(maxBounds.z - minBounds.z);
}

void SceneNode::computeAABB(std::shared_ptr<ofxAssimpModelLoader> model) {
    ofVec3f minBounds(FLT_MAX, FLT_MAX, FLT_MAX);
    ofVec3f maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < model->getNumMeshes(); i++) {
        ofMesh mesh = model->getMesh(i);

        for (const auto& vertex : mesh.getVertices()) {
            ofVec4f transformedVertex = model->getModelMatrix() * ofVec4f(vertex.x, vertex.y, vertex.z, 1.0f);

            minBounds.x = std::min(minBounds.x, transformedVertex.x);
            minBounds.y = std::min(minBounds.y, transformedVertex.y);
            minBounds.z = std::min(minBounds.z, transformedVertex.z);

            maxBounds.x = std::max(maxBounds.x, transformedVertex.x);
            maxBounds.y = std::max(maxBounds.y, transformedVertex.y);
            maxBounds.z = std::max(maxBounds.z, transformedVertex.z);
        }
    }

    aabb.setPosition((minBounds + maxBounds) * 0.5f);
    aabb.setWidth(maxBounds.x - minBounds.x);
    aabb.setHeight(maxBounds.y - minBounds.y);
    aabb.setDepth(maxBounds.z - minBounds.z);
}

bool SceneNode::isAABBVisible(const ofCamera& camera) {
    glm::mat4 globalTransform = getGlobalTransformMatrix();
    auto vertices = getTransformedAABBVertices(globalTransform);

    auto frustumPlanes = extractFrustumPlanes(camera);

    return isAABBInsideFrustum(vertices, frustumPlanes);
}

std::vector<glm::vec4> SceneNode::extractFrustumPlanes(const ofCamera& camera) {
    glm::mat4 viewProj = camera.getModelViewProjectionMatrix();

    std::vector<glm::vec4> planes(6);
    planes[0] = glm::vec4(viewProj[0][3] + viewProj[0][0], viewProj[1][3] + viewProj[1][0], viewProj[2][3] + viewProj[2][0], viewProj[3][3] + viewProj[3][0]); // Left
    planes[1] = glm::vec4(viewProj[0][3] - viewProj[0][0], viewProj[1][3] - viewProj[1][0], viewProj[2][3] - viewProj[2][0], viewProj[3][3] - viewProj[3][0]); // Right
    planes[2] = glm::vec4(viewProj[0][3] + viewProj[0][1], viewProj[1][3] + viewProj[1][1], viewProj[2][3] + viewProj[2][1], viewProj[3][3] + viewProj[3][1]); // Bottom
    planes[3] = glm::vec4(viewProj[0][3] - viewProj[0][1], viewProj[1][3] - viewProj[1][1], viewProj[2][3] - viewProj[2][1], viewProj[3][3] - viewProj[3][1]); // Top
    planes[4] = glm::vec4(viewProj[0][3] + viewProj[0][2], viewProj[1][3] + viewProj[1][2], viewProj[2][3] + viewProj[2][2], viewProj[3][3] + viewProj[3][2]); // Near
    planes[5] = glm::vec4(viewProj[0][3] - viewProj[0][2], viewProj[1][3] - viewProj[1][2], viewProj[2][3] - viewProj[2][2], viewProj[3][3] - viewProj[3][2]); // Far

    return planes;
}

std::vector<glm::vec3> SceneNode::getAABBVertices() {
    ofVec3f center = aabb.getPosition();
    float halfWidth = aabb.getWidth() / 2.0f;
    float halfHeight = aabb.getHeight() / 2.0f;
    float halfDepth = aabb.getDepth() / 2.0f;

    std::vector<glm::vec3> vertices(8);
    vertices[0] = glm::vec3(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth);
    vertices[1] = glm::vec3(center.x + halfWidth, center.y - halfHeight, center.z - halfDepth);
    vertices[2] = glm::vec3(center.x - halfWidth, center.y + halfHeight, center.z - halfDepth);
    vertices[3] = glm::vec3(center.x + halfWidth, center.y + halfHeight, center.z - halfDepth);
    vertices[4] = glm::vec3(center.x - halfWidth, center.y - halfHeight, center.z + halfDepth);
    vertices[5] = glm::vec3(center.x + halfWidth, center.y - halfHeight, center.z + halfDepth);
    vertices[6] = glm::vec3(center.x - halfWidth, center.y + halfHeight, center.z + halfDepth);
    vertices[7] = glm::vec3(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth);

    return vertices;
}

std::vector<glm::vec3> SceneNode::getTransformedAABBVertices(const glm::mat4& transform) {
    auto vertices = getAABBVertices();
    std::vector<glm::vec3> transformedVertices;

    for (const auto& vertex : vertices) {
        glm::vec4 transformedVertex = transform * glm::vec4(vertex, 1.0f);
        transformedVertices.push_back(glm::vec3(transformedVertex));
    }

    return transformedVertices;
}

bool SceneNode::isAABBInsideFrustum(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec4>& frustumPlanes) {
    for (const auto& plane : frustumPlanes) {
        bool allOutside = true;

        for (const auto& vertex : vertices) {
            float distance = plane.x * vertex.x + plane.y * vertex.y + plane.z * vertex.z + plane.w;
            if (distance >= 0) {
                allOutside = false;
                break;
            }
        }

        if (allOutside) {
            return false;
        }
    }

    return true;
}

void SceneNode::drawAABBBox() {
    auto vertices = getAABBVertices();

    ofDrawLine(vertices[0], vertices[1]);
    ofDrawLine(vertices[1], vertices[3]);
    ofDrawLine(vertices[3], vertices[2]);
    ofDrawLine(vertices[2], vertices[0]);

    ofDrawLine(vertices[4], vertices[5]);
    ofDrawLine(vertices[5], vertices[7]);
    ofDrawLine(vertices[7], vertices[6]);
    ofDrawLine(vertices[6], vertices[4]);

    ofDrawLine(vertices[0], vertices[4]);
    ofDrawLine(vertices[1], vertices[5]);
    ofDrawLine(vertices[2], vertices[6]);
    ofDrawLine(vertices[3], vertices[7]);
}

bool SceneNode::operator==(const SceneNode& other) const {
    return (name == other.name);
}
