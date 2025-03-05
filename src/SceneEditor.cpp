#include "SceneEditor.h"

void SceneEditor::setup() {
    sceneName = "Scene";

    sceneGraph.setup();

    gizmoManager.setup();
    gizmoManager.setSelectedNode(sceneGraph.getSelectedNode());

    cameraManager.setup();
    cameraManager.addOrbitCamera("Orbit Camera", glm::vec3(0, 0, 500), glm::vec3(0, 0, 0));
    cameraManager.addFreeFlightCamera("Free Flight Camera", glm::vec3(0, 0, 500));
    cameraManager.setSelectedCamera(0);

    light.setup();
    light.setPosition(200, 300, 400);
    light.enable();

    material.setDiffuseColor(ofFloatColor(0.5f, 0.5f, 0.5f, 1.0f));
    material.setAmbientColor(ofFloatColor(0.3f, 0.3f, 0.3f, 1.0f));
    material.setSpecularColor(ofFloatColor(0.9f, 0.9f, 0.9f, 1.0f));
    material.setEmissiveColor(ofFloatColor(0.1f, 0.1f, 0.1f, 1.0f));
    material.setShininess(64);

    justAddedNode = false;
    shouldEnableMouseInput = false;
}

void SceneEditor::update() {
    light.setPosition(cameraManager.getSelectedCamera()->getPosition());

    sceneGraph.update();
    gizmoManager.update();
    cameraManager.update();
}

void SceneEditor::draw() {
    ofEnableDepthTest();
    ofEnableLighting();

    if (shouldEnableMouseInput) {
        cameraManager.enableSelectedMouseInput();
    }

    ofCamera* selectedCam = cameraManager.getSelectedCamera();
    ofCamera* frustumCam = cameraManager.getFrustumCamera();

    selectedCam->begin();

    light.enable();
    material.begin();

    if (cameraManager.isFrustumCullingEnabled() && frustumCam) {
        sceneGraph.drawVisibleNodes(*frustumCam);
    }
    else {
        sceneGraph.draw();
    }

    material.end();
    light.disable();

    gizmoManager.draw(*selectedCam);

    if (frustumCam) {
        ofPushStyle();
        ofSetColor(ofColor::yellow);

        frustumCam->drawFrustum();

        ofPopStyle();
    }

    selectedCam->end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void SceneEditor::drawGui() {
    ImGui::Begin(sceneName);

    drawSceneGraph();

    ImGui::End();

    gizmoManager.drawGui();
    cameraManager.drawGui();

    drawInfo(sceneGraph.getSelectedNode());
}

void SceneEditor::drawInfo(std::shared_ptr<SceneNode> node) {
    ImGui::Begin("Node Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);

    static char name[255];
    strncpy(name, node->getName().c_str(), sizeof(name) - 1);
    if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
        sceneGraph.editNodeName(node, std::string(name));
    }

    glm::vec3 position = node->getPosition();
    if (ImGui::InputFloat3("Position", &position.x) && ImGui::IsItemDeactivatedAfterEdit()) {
        node->setPosition(position);
        gizmoManager.setSelectedNode(node);
    }

    glm::vec3 scale = node->getScale();
    if (ImGui::InputFloat3("Scale", &scale.x) && ImGui::IsItemDeactivatedAfterEdit()) {
        node->setScale(scale);
        gizmoManager.setSelectedNode(node);
    }

    glm::vec3 rotation = node->getOrientationEuler();
    if (ImGui::InputFloat3("Rotation", &rotation.x) && ImGui::IsItemDeactivatedAfterEdit()) {
        node->setOrientation(rotation);
        gizmoManager.setSelectedNode(node);
    }

    ImGui::End();
}

void SceneEditor::drawSceneGraph() {
    drawSceneGraphNode(sceneGraph.getRootNode());
}

void SceneEditor::drawSceneGraphNode(std::shared_ptr<SceneNode> node) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (node == sceneGraph.getSelectedNode()) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
        if (justAddedNode) {
            ImGui::SetNextTreeNodeOpen(true);
            justAddedNode = false;
        }
    }

    bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), nodeFlags);
    nodeDragDropBehaviour(node);

    if (ImGui::IsItemClicked()) {
        sceneGraph.setSelectedNode(node);
        gizmoManager.setSelectedNode(node);
    }

    if (ImGui::BeginPopupContextItem()) {
        newObjectMenu(node);
        ImGui::EndPopup();
    }

    if (nodeOpen) {
        for (auto& child : node->getChildren()) {
            drawSceneGraphNode(child);
        }

        ImGui::TreePop();
    }
}

void SceneEditor::newObjectMenu(std::shared_ptr<SceneNode> node) {
    if (node != sceneGraph.getRootNode()) {
        if (ImGui::Selectable("Delete")) {
            sceneGraph.deleteNode(node);
        }
    }

    if (ImGui::Selectable("New Empty")) {
        newEmptyObject("Empty", node);
    }

    if (ImGui::Selectable("New Sphere")) {
        newPrimitiveObject(PrimitiveType::Sphere, "Sphere", node);
    }

    if (ImGui::Selectable("New Cube")) {
        newPrimitiveObject(PrimitiveType::Cube, "Cube", node);
    }

    if (ImGui::Selectable("New Cylinder")) {
        newPrimitiveObject(PrimitiveType::Cylinder, "Cylinder", node);
    }

    if (ImGui::Selectable("New Cone")) {
        newPrimitiveObject(PrimitiveType::Cone, "Cone", node);
    }
}

void SceneEditor::nodeDragDropBehaviour(std::shared_ptr<SceneNode> node) {
    if (ImGui::BeginDragDropSource()) {
        SceneNode* nodePointer = node.get();
        ImGui::SetDragDropPayload("MOVE_NODE", &nodePointer, sizeof(SceneNode*));
        ImGui::Text("%s", node->getName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MOVE_NODE")) {
            SceneNode* sourceNode = *(SceneNode**)payload->Data;
            sceneGraph.transferNode(sourceNode->shared_from_this(), node);
        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET")) {
            const char* asset = (const char*)payload->Data;
            load(asset);
        }

        ImGui::EndDragDropTarget();
    }
}

void SceneEditor::newEmptyObject(const std::string& name, std::shared_ptr<SceneNode> parent) {
    sceneGraph.addEmptyNode(name, parent);
    justAddedNode = true;
}

void SceneEditor::newPrimitiveObject(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent) {
    sceneGraph.addPrimitiveNode(primitiveType, name, parent);
    justAddedNode = true;
}

void SceneEditor::exit() {
    cameraManager.disableAllMouseInput();
    shouldEnableMouseInput = true;

    sceneGraph.exit();
    gizmoManager.exit();
    cameraManager.exit();
}

void SceneEditor::mouseDragged(int x, int y, int button) {

}

void SceneEditor::mousePressed(int x, int y, int button) {

}

void SceneEditor::mouseReleased(int x, int y, int button) {

}

void SceneEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}

void SceneEditor::load(const std::string& path) {
    sceneGraph.addModelNode(path, sceneGraph.getRootNode());
}

void SceneEditor::save(const std::string& path) {

}
