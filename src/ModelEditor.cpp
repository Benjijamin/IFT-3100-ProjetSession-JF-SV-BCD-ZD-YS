#include "ModelEditor.h"

void ModelEditor::setup() {
    light.setup();
    light.setPosition(200, 300, 400);
    light.enable();

    material.setDiffuseColor(ofFloatColor::gray);
    material.setSpecularColor(ofFloatColor::white);
    material.setShininess(32);

    sceneGraph.setup();
    gizmo.setType(ofxGizmo::OFX_GIZMO_MOVE);
    gizmo.hide();

    switchToOrbitCamera();
    shouldEnableMouseInput = false;

    sceneGraph.setSelectedNodeChangedCallback([this](std::shared_ptr<SceneNode> selectedNode) {
        if (selectedNode) {
            gizmo.setNode(*selectedNode);
        }
        });
}

void ModelEditor::update() {
    if (isFreeFlightMode) {
        light.setPosition(freeFlightCam.getPosition());
    }
    else {
        light.setPosition(orbitCam.getPosition());
    }

    sceneGraph.update();
    updateGizmo();
}

void ModelEditor::draw() {
    ofEnableDepthTest();
    ofEnableLighting();

    if (shouldEnableMouseInput) {
        if (isFreeFlightMode) {
            orbitCam.enableMouseInput();
        }
        else {
            freeFlightCam.enableMouseInput();
        }
    }

    ofCamera* activeCam = getActiveCamera();

    activeCam->begin();

    light.enable();
    material.begin();

    sceneGraph.draw();

    material.end();
    light.disable();

    bool isOrtho = activeCam->getOrtho();

    activeCam->disableOrtho();

    gizmo.draw(*activeCam);

    if (isOrtho) {
        activeCam->enableOrtho();
    }

    activeCam->end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void ModelEditor::drawGui() {
    sceneGraph.drawGui();

    drawGizmoControls();
    drawCameraControls();
}

void ModelEditor::drawGizmoControls() {
    auto selectedNode = sceneGraph.getSelectedNode();

    if (selectedNode) {
        ImGui::Begin("Gizmo Controls");

        ImGui::Text("Gizmo Controls for Node: %s", selectedNode->getName().c_str());

        bool translateSelected = (gizmoType == ofxGizmo::OFX_GIZMO_MOVE);
        bool rotateSelected = (gizmoType == ofxGizmo::OFX_GIZMO_ROTATE);
        bool scaleSelected = (gizmoType == ofxGizmo::OFX_GIZMO_SCALE);

        if (ImGui::RadioButton("Translate", translateSelected)) {
            gizmoType = ofxGizmo::OFX_GIZMO_MOVE;
        }

        if (ImGui::RadioButton("Rotate", rotateSelected)) {
            gizmoType = ofxGizmo::OFX_GIZMO_ROTATE;
        }

        if (ImGui::RadioButton("Scale", scaleSelected)) {
            gizmoType = ofxGizmo::OFX_GIZMO_SCALE;
        }

        gizmo.setType(gizmoType);
        gizmo.show();

        ImGui::End();
    }
    else {
        gizmo.hide();
    }
}

void ModelEditor::drawCameraControls() {
    ImGui::Begin("Camera Controls");

    if (ImGui::RadioButton("Orbit Mode", !isFreeFlightMode)) {
        switchToOrbitCamera();
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Free Flight Mode", isFreeFlightMode)) {
        switchToFreeFlightCamera();
    }

    ofCamera* activeCam = getActiveCamera();

    static bool isPerspective = activeCam->getOrtho() == false;

    if (ImGui::RadioButton("Perspective", isPerspective)) {
        isPerspective = true;
        activeCam->disableOrtho();
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Orthographic", !isPerspective)) {
        isPerspective = false;
        activeCam->enableOrtho();
    }

    if (isFreeFlightMode) {
        drawFreeFlightParameters();
    }

    if (isPerspective) {
        drawPerspectiveParameters();
    }
    else {
        drawOrthographicParameters();
    }

    ImGui::End();
}

void ModelEditor::drawFreeFlightParameters() {
    if (ImGui::CollapsingHeader("Free Flight Parameters")) {
        float moveSpeed = freeFlightCam.getMoveSpeed();
        if (ImGui::SliderFloat("Move Speed", &moveSpeed, 1.0f, 50.0f)) {
            freeFlightCam.setMoveSpeed(moveSpeed);
        }

        float sensitivity = freeFlightCam.getSensitivity();
        if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.01f, 1.0f)) {
            freeFlightCam.setSensitivity(sensitivity);
        }
    }
}

void ModelEditor::drawPerspectiveParameters() {
    if (ImGui::CollapsingHeader("Perspective Parameters")) {
        ofCamera* activeCam = getActiveCamera();

        float fov = activeCam->getFov();
        if (ImGui::SliderFloat("Field of View", &fov, 30.0f, 120.0f)) {
            activeCam->setFov(fov);
        }

        float aspect = activeCam->getAspectRatio();
        if (ImGui::SliderFloat("Aspect Ratio", &aspect, 1.0f, 2.0f)) {
            activeCam->setAspectRatio(aspect);
        }

        float nearClip = activeCam->getNearClip();
        if (ImGui::SliderFloat("Near Clip", &nearClip, 0.01f, 10.0f)) {
            activeCam->setNearClip(nearClip);
        }

        float farClip = activeCam->getFarClip();
        if (ImGui::SliderFloat("Far Clip", &farClip, 100.0f, 10000.0f)) {
            activeCam->setFarClip(farClip);
        }
    }
}

void ModelEditor::drawOrthographicParameters() {
    if (ImGui::CollapsingHeader("Orthographic Parameters")) {
        ofCamera* activeCam = getActiveCamera();

        float nearClip = activeCam->getNearClip();
        if (ImGui::SliderFloat("Near Clip", &nearClip, 0.01f, 10.0f)) {
            activeCam->setNearClip(nearClip);
        }

        float farClip = activeCam->getFarClip();
        if (ImGui::SliderFloat("Far Clip", &farClip, 100.0f, 10000.0f)) {
            activeCam->setFarClip(farClip);
        }
    }
}

void ModelEditor::exit() {
    orbitCam.disableMouseInput();
    freeFlightCam.disableMouseInput();

    shouldEnableMouseInput = true;
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

void ModelEditor::unload(const std::string& path) {
    sceneGraph.unloadNodes(path);
}

void ModelEditor::save(const std::string& path) {

}

void ModelEditor::switchToOrbitCamera() {
    freeFlightCam.disableMouseInput();
    orbitCam.enableMouseInput();
    orbitCam.setTarget(glm::vec3(0.0f));
    isFreeFlightMode = false;
}

void ModelEditor::switchToFreeFlightCamera() {
    orbitCam.disableMouseInput();
    freeFlightCam.enableMouseInput();
    isFreeFlightMode = true;
}

void ModelEditor::updateGizmo() {
    auto selectedNode = sceneGraph.getSelectedNode();

    if (selectedNode) {
        gizmo.apply(*selectedNode);
    }
}

ofCamera* ModelEditor::getActiveCamera() {
    return isFreeFlightMode ? static_cast<ofCamera*>(&freeFlightCam) : static_cast<ofCamera*>(&orbitCam);
}
