#include "ModelEditor.h"
#include <filesystem>

void ModelEditor::setup() {
    light.setup();
    light.setPosition(200, 300, 400);
    light.enable();

    material.setDiffuseColor(ofFloatColor::gray);
    material.setSpecularColor(ofFloatColor::white);
    material.setShininess(64);

    ofDisableDepthTest();
    ofDisableLighting();

    sceneGraph.setup();
}

void ModelEditor::update() {
    light.setPosition(camera.getPosition());
    sceneGraph.update();
}

void ModelEditor::draw() {
    ofEnableDepthTest();
    ofEnableLighting();

    camera.begin();

    light.enable();
    material.begin();

    sceneGraph.draw(camera); // Draw the scene graph

    material.end();
    light.disable();

    camera.end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void ModelEditor::drawGui() {
    sceneGraph.drawGui();
}

void ModelEditor::exit() {
    camera.disableMouseInput();
}

void ModelEditor::loadModel(const std::string& path) {
    // Load the model from the specified path
    auto model = std::make_shared<ofxAssimpModelLoader>();
    model->load(path);
    model->setPosition(0, 0, 0);

    // Extract the stem to use as the node name
    std::string stem = std::filesystem::path(path).stem().string();
    sceneGraph.addModelNode(stem, model);

    camera.enableMouseInput();
}
