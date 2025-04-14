#ifndef AABB_H
#define AABB_H

#include <vector>
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class AABB : public ofBoxPrimitive {
public:
    AABB();

    void initToInfinity();
    void compute(const std::shared_ptr<ofxAssimpModelLoader>& model);
    void setBounds(const ofVec3f& minBounds, const ofVec3f& maxBounds);

    ofBoxPrimitive getBox() const;

    bool isVisible(const ofCamera& camera, const glm::mat4& globalTransform) const;

    void draw() const;

private:
    bool isInsideFrustum(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec4>& frustumPlanes) const;

    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getTransformedVertices(const glm::mat4& transform) const;
    std::vector<glm::vec4> calculateFrustumPlanes(const glm::mat4& viewProj) const;
};

#endif // AABB_H
