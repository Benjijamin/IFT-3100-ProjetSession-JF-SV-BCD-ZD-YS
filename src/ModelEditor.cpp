#include "ModelEditor.h"

void ModelEditor::setup() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    light.setup();
    light.setPosition(200, 300, 400);
    light.enable();

    material.setDiffuseColor(ofFloatColor::gray);
    material.setSpecularColor(ofFloatColor::white);
    material.setShininess(64);

    ofDisableDepthTest();
    ofDisableLighting();
}

void ModelEditor::update() {
    light.setPosition(camera.getPosition());
}

void ModelEditor::draw() {
    camera.begin();

    ofEnableDepthTest();
    ofEnableLighting();

    light.enable();

    ofPushMatrix();
    ofTranslate(translation);
    ofRotateDeg(rotation.x, 1, 0, 0);
    ofRotateDeg(rotation.y, 0, 1, 0);
    ofRotateDeg(rotation.z, 0, 0, 1);
    ofScale(scale, scale, scale);

    material.begin();
    if (model.getNumMeshes() > 0) {
        model.drawFaces();
    }
    material.end();

    ofPopMatrix();

    light.disable();

    ofDisableDepthTest();
    ofDisableLighting();

    camera.end();
}

void ModelEditor::drawGui() {
    ImGui::Begin("Model Editor");

    ImGui::SliderFloat3("Translate", translation.getPtr(), -200.0f, 200.0f);
    ImGui::SliderFloat3("Rotate", rotation.getPtr(), 0.0f, 360.0f);
    ImGui::SliderFloat("Scale", &scale, 0.1f, 10.0f);

    ImGui::End();
}

void ModelEditor::exit() {
    camera.disableMouseInput();
}

void ModelEditor::loadModel(const std::string& path) {
    model.load(path);
    model.setPosition(0, 0, 0);

    camera.enableMouseInput();
}
