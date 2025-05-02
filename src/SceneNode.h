#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"
#include "AABB.h"
#include "Primitives.h"
#include "BezierCurve.h"

enum class PrimitiveType {
    None, Sphere, Tetrahedron, Cube, Cylinder, Cone
};

class SceneNode : public ofNode, public std::enable_shared_from_this<SceneNode> {
public:
    SceneNode(const std::string& name);
    ~SceneNode();

    void addChild(std::shared_ptr<SceneNode> child);
    void removeChild(std::shared_ptr<SceneNode> child);
    std::vector<std::shared_ptr<SceneNode>> getChildren() const;

    std::string getName() const;
    void setName(const std::string& newName);

    void setPrimitive(PrimitiveType newPrimitiveType);

    void setModel(std::shared_ptr<ofxAssimpModelLoader> newModel);
    bool hasModel() const;

    void setDefaultMaterial();
    void setLightMaterial();
    bool hasMaterial() const;
    std::shared_ptr<ofMaterial> getMaterial() const;

    void setLight(std::shared_ptr<ofLight> newLight);
    bool hasLight() const;
    std::shared_ptr<ofLight> getLight() const;

    void setSurface(std::shared_ptr<ofMesh> surfaceMesh);
    void updateSurface();
    bool isSurfaceControl() const;
    std::vector<glm::vec3> getControlPoints() const;


    void setTexture(const std::string& path);

    void draw();
    void drawVisibleNodes(const ofCamera& camera);

    ofBoxPrimitive getBoundingBox() const;

    bool operator==(const SceneNode& other) const;

    PrimitiveType getPrimitiveType() const {return primitiveType;}


private:
    void customDraw() override;

    void drawLight();
    void drawPrimitive();
    void drawSurface();

    std::string name;
    std::vector<std::shared_ptr<SceneNode>> children;

    PrimitiveType primitiveType;
    std::shared_ptr<ofMesh> primitiveModel;

    ofImage textureImage;
    ofTexture texture;

    std::shared_ptr<ofxAssimpModelLoader> model;
    std::shared_ptr<ofMaterial> material;
    std::shared_ptr<ofLight> light;

    std::shared_ptr<ofMesh> surface;

    SceneNode* parent = nullptr;
    AABB boundingBox;
};
