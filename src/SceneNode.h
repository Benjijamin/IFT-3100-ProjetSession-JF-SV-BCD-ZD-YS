#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"

enum class PrimitiveType 
{
    None, Sphere, Tetrahedron, Cube, Cylinder, Cone 
};

class SceneNode : public ofNode, public std::enable_shared_from_this<SceneNode> {
public:
    SceneNode(const std::string& name);

    void setModel(std::shared_ptr<ofxAssimpModelLoader> model);
    void draw();
    void drawVisibleNodes(const ofCamera& camera);

    void addChild(std::shared_ptr<SceneNode> child);
    void removeChild(std::shared_ptr<SceneNode> child);
    std::vector<std::shared_ptr<SceneNode>> getChildren() const;

    std::string getName() const;
    void setName(std::string& newName);

    void setPrimitive(PrimitiveType newPrimitiveType);
    bool containsModel() const;

    ofBoxPrimitive getAABB() const;

    bool operator==(const SceneNode& other) const;

private:
    void customDraw() override;

    void initAABBToInfinity();
    void computeAABB(std::shared_ptr<ofxAssimpModelLoader> model);
    bool isAABBVisible(const ofCamera& camera);
    std::vector<glm::vec4> extractFrustumPlanes(const ofCamera& camera);
    std::vector<glm::vec3> getAABBVertices();
    std::vector<glm::vec3> SceneNode::getTransformedAABBVertices(const glm::mat4& transform);
    bool isAABBInsideFrustum(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec4>& frustumPlanes);
    void drawAABBBox();

    std::string name;
    std::shared_ptr<ofxAssimpModelLoader> model;
    std::vector<std::shared_ptr<SceneNode>> children;

    PrimitiveType primitiveType;
    std::shared_ptr<ofMesh> primitiveModel;

    ofBoxPrimitive aabb;
};
