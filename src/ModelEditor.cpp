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
    switchToOrbitCamera();
}

void ModelEditor::update() {
    if (isFreeFlightMode) {
        light.setPosition(freeFlightCam.getPosition());
    }
    else {
        light.setPosition(orbitCam.getPosition());
    }

    sceneGraph.update();
}

void ModelEditor::draw() {
    ofEnableDepthTest();
    ofEnableLighting();

    ofCamera* activeCam = isFreeFlightMode ? static_cast<ofCamera*>(&freeFlightCam) : static_cast<ofCamera*>(&orbitCam);

    activeCam->begin();

    light.enable();
    material.begin();

    sceneGraph.draw(*activeCam);

    material.end();
    light.disable();

    activeCam->end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void ModelEditor::drawGui() {
    sceneGraph.drawGui();

    ImGui::Begin("Camera Controls");

    if (ImGui::RadioButton("Orbit Mode", !isFreeFlightMode)) {
        switchToOrbitCamera();
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Free Flight Mode", isFreeFlightMode)) {
        switchToFreeFlightCamera();
    }

    if (isFreeFlightMode) {
        float moveSpeed = freeFlightCam.getMoveSpeed();
        if (ImGui::SliderFloat("Move Speed", &moveSpeed, 1.0f, 50.0f)) {
            freeFlightCam.setMoveSpeed(moveSpeed);
        }

        float sensitivity = freeFlightCam.getSensitivity();
        if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.01f, 1.0f)) {
            freeFlightCam.setSensitivity(sensitivity);
        }
    }

    ImGui::End();
}

void ModelEditor::exit() {
    orbitCam.disableMouseInput();
    freeFlightCam.disableMouseInput();
}

void ModelEditor::mouseDragged(int x, int y, int button) {

}

void ModelEditor::mousePressed(int x, int y, int button) {

}

void ModelEditor::mouseReleased(int x, int y, int button) {

}

void ModelEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}

void ModelEditor::load(const std::string& path) {
    auto model = std::make_shared<ofxAssimpModelLoader>();
    model->load(path);
    model->setPosition(0, 0, 0);

    std::string stem = std::filesystem::path(path).stem().string();
    sceneGraph.addModelNode(stem, model);

    if (!isFreeFlightMode) {
        orbitCam.enableMouseInput();
    }
}

void ModelEditor::save(const std::string& path) {

}

void ModelEditor::switchToOrbitCamera() {
    orbitCam.setTarget(glm::vec3(0.0f));
    orbitCam.enableMouseInput();
    isFreeFlightMode = false;
}

void ModelEditor::switchToFreeFlightCamera() {
    freeFlightCam.enableMouseInput();
    isFreeFlightMode = true;
}
