#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "SceneGraph.h"

class ModelEditor {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void loadModel(const std::string& path);

private:
    ofLight light;
    ofMaterial material;
    ofEasyCam camera;
    SceneGraph sceneGraph;
};
