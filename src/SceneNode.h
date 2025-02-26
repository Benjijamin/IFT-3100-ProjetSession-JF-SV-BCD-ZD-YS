#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"

class SceneNode : public ofNode, public std::enable_shared_from_this<SceneNode> {
public:
    SceneNode(const std::string& name);

    void setModel(std::shared_ptr<ofxAssimpModelLoader> model);
    void draw();

    void addChild(std::shared_ptr<SceneNode> child);
    void removeChild(std::shared_ptr<SceneNode> child);
    std::vector<std::shared_ptr<SceneNode>> getChildren() const;

    std::string getName() const;
    void setName(std::string& newName);
    bool containsModel() const;

    bool operator ==(const SceneNode& other) const;

private:
    std::string name;
    std::shared_ptr<ofxAssimpModelLoader> model;
    std::vector<std::shared_ptr<SceneNode>> children;

    void customDraw() override;
};
