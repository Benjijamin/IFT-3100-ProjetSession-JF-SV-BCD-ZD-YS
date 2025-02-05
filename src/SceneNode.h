#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"

class SceneNode : public ofNode {
public:
    SceneNode(const std::string& name);

    void setModel(std::shared_ptr<ofxAssimpModelLoader> model);
    void customDraw() override;

    std::string getName() const;

private:
    std::string name;
    std::shared_ptr<ofxAssimpModelLoader> model;
};
