#include "AABB.h"

AABB::AABB() {
    initToInfinity();
}

void AABB::initToInfinity() {
    ofVec3f minBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    ofVec3f maxBounds(FLT_MAX, FLT_MAX, FLT_MAX);
    setBounds(minBounds, maxBounds);
}

void AABB::compute(const std::shared_ptr<ofxAssimpModelLoader>& model) {
    ofVec3f minBounds(FLT_MAX, FLT_MAX, FLT_MAX);
    ofVec3f maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < model->getNumMeshes(); ++i) {
        const ofMesh& mesh = model->getMesh(i);

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

    setBounds(minBounds, maxBounds);
}

void AABB::setBounds(const ofVec3f& minBounds, const ofVec3f& maxBounds) {
    setPosition((minBounds + maxBounds) * 0.5f);
    setWidth(maxBounds.x - minBounds.x);
    setHeight(maxBounds.y - minBounds.y);
    setDepth(maxBounds.z - minBounds.z);
}

ofBoxPrimitive AABB::getBox() const {
    return *this;
}

bool AABB::isVisible(const ofCamera& camera, const glm::mat4& globalTransform) const {
    auto vertices = getTransformedVertices(globalTransform);
    auto frustumPlanes = calculateFrustumPlanes(camera.getModelViewProjectionMatrix());
    return isInsideFrustum(vertices, frustumPlanes);
}

bool AABB::isInsideFrustum(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec4>& frustumPlanes) const {
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

std::vector<glm::vec3> AABB::getVertices() const {
    ofVec3f center = getPosition();
    float halfWidth = getWidth() / 2.0f;
    float halfHeight = getHeight() / 2.0f;
    float halfDepth = getDepth() / 2.0f;

    return {
        {center.x - halfWidth, center.y - halfHeight, center.z - halfDepth},
        {center.x + halfWidth, center.y - halfHeight, center.z - halfDepth},
        {center.x - halfWidth, center.y + halfHeight, center.z - halfDepth},
        {center.x + halfWidth, center.y + halfHeight, center.z - halfDepth},
        {center.x - halfWidth, center.y - halfHeight, center.z + halfDepth},
        {center.x + halfWidth, center.y - halfHeight, center.z + halfDepth},
        {center.x - halfWidth, center.y + halfHeight, center.z + halfDepth},
        {center.x + halfWidth, center.y + halfHeight, center.z + halfDepth},
    };
}

std::vector<glm::vec3> AABB::getTransformedVertices(const glm::mat4& transform) const {
    auto vertices = getVertices();
    std::vector<glm::vec3> transformedVertices;

    for (const auto& vertex : vertices) {
        glm::vec4 transformedVertex = transform * glm::vec4(vertex, 1.0f);
        transformedVertices.push_back(glm::vec3(transformedVertex));
    }

    return transformedVertices;
}

std::vector<glm::vec4> AABB::calculateFrustumPlanes(const glm::mat4& viewProj) const {
    return {
        glm::vec4(viewProj[0][3] + viewProj[0][0], viewProj[1][3] + viewProj[1][0], viewProj[2][3] + viewProj[2][0], viewProj[3][3] + viewProj[3][0]), // Left
        glm::vec4(viewProj[0][3] - viewProj[0][0], viewProj[1][3] - viewProj[1][0], viewProj[2][3] - viewProj[2][0], viewProj[3][3] - viewProj[3][0]), // Right
        glm::vec4(viewProj[0][3] + viewProj[0][1], viewProj[1][3] + viewProj[1][1], viewProj[2][3] + viewProj[2][1], viewProj[3][3] + viewProj[3][1]), // Bottom
        glm::vec4(viewProj[0][3] - viewProj[0][1], viewProj[1][3] - viewProj[1][1], viewProj[2][3] - viewProj[2][1], viewProj[3][3] - viewProj[3][1]), // Top
        glm::vec4(viewProj[0][3] + viewProj[0][2], viewProj[1][3] + viewProj[1][2], viewProj[2][3] + viewProj[2][2], viewProj[3][3] + viewProj[3][2]), // Near
        glm::vec4(viewProj[0][3] - viewProj[0][2], viewProj[1][3] - viewProj[1][2], viewProj[2][3] - viewProj[2][2], viewProj[3][3] - viewProj[3][2])  // Far
    };
}

void AABB::draw() const {
    auto vertices = getVertices();

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
