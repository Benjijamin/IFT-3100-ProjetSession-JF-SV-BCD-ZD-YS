#include "GizmoManager.h"

void GizmoManager::setup() {
    gizmo.setType(ofxGizmo::OFX_GIZMO_MOVE);
    gizmo.hide();
}

void GizmoManager::update() {
    if (selectedNode) {
        gizmo.apply(*selectedNode);
    }
}

void GizmoManager::draw(ofCamera& camera) {
    gizmo.draw(camera);
}

void GizmoManager::drawGui() {
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

void GizmoManager::exit() {

}

void GizmoManager::setSelectedNode(std::shared_ptr<SceneNode> node) {
    selectedNode = node;

    if (selectedNode) {
        gizmo.setNode(*selectedNode);
    }
    else {
        gizmo.hide();
    }
}
