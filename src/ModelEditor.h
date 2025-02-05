#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxImGui.h"

class ModelEditor {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void loadModel(const std::string& path);

private:
    ofxAssimpModelLoader model;
    ofEasyCam camera;
    ofLight light;
    ofMaterial material;

    ofVec3f translation = ofVec3f(0.0f, 0.0f, 0.0f);
    ofVec3f rotation = ofVec3f(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
};
