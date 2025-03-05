#include "ModelEditor.h"

void ModelEditor::setup() {
    light.setup();
    light.setPosition(200, 300, 400);
    light.enable();

    material.setDiffuseColor(ofFloatColor::gray);
    material.setSpecularColor(ofFloatColor::white);
    material.setShininess(32);

    sceneGraph.setup();
    gizmoManager.setup();

    cameraManager.setup();
    cameraManager.addOrbitCamera("Orbit Camera", glm::vec3(0, 0, 500), glm::vec3(0, 0, 0));
    cameraManager.addFreeFlightCamera("Free Flight Camera", glm::vec3(0, 0, 500));
    cameraManager.setSelectedCamera(0);

    shouldEnableMouseInput = false;

    sceneGraph.setSelectedNodeChangedCallback([this](std::shared_ptr<SceneNode> selectedNode) {
        gizmoManager.setSelectedNode(selectedNode);
    });
}

void ModelEditor::update() {
    light.setPosition(cameraManager.getSelectedCamera()->getPosition());

    sceneGraph.update();
    gizmoManager.update();
    cameraManager.update();
}

void ModelEditor::draw() {
    ofEnableDepthTest();
    ofEnableLighting();

    if (shouldEnableMouseInput) {
        cameraManager.enableSelectedMouseInput();
    }

    ofCamera* activeCam = cameraManager.getSelectedCamera();
    activeCam->begin();

    light.enable();
    material.begin();

    sceneGraph.draw();

    material.end();
    light.disable();

    gizmoManager.draw(*activeCam);
    cameraManager.draw();

    activeCam->end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void ModelEditor::drawGui() {
    sceneGraph.drawGui();
    gizmoManager.drawGui();
    cameraManager.drawGui();
}

void ModelEditor::exit() {
    cameraManager.disableAllMouseInput();
    shouldEnableMouseInput = true;

    sceneGraph.exit();
    gizmoManager.exit();
    cameraManager.exit();
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
    sceneGraph.addModelNode(path);
}

void ModelEditor::save(const std::string& path) {

}
