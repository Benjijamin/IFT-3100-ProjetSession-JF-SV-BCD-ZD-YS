#include "SceneEditor.h"

void SceneEditor::setup() {
    sceneName = "Scene";

    sceneGraph.setup();

    auto defaultLight = std::make_shared<ofLight>();

    defaultLight->enable();
    defaultLight->setPointLight();
    defaultLight->setDiffuseColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
    defaultLight->setAmbientColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
    defaultLight->setSpecularColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));

    sceneGraph.addLightNode(defaultLight, "Default Light", sceneGraph.getRootNode());

    gizmoManager.setup();
    gizmoManager.setSelectedNode(sceneGraph.getSelectedNode());

    cameraManager.setup();
    cameraManager.addOrbitCamera("Orbit Camera", glm::vec3(0, 0, 500), glm::vec3(0, 0, 0));
    cameraManager.addFreeFlightCamera("Free Flight Camera", glm::vec3(0, 0, 500));
    cameraManager.setSelectedCamera(0);

    firstTime = true;
    justAddedNode = false;
    shouldEnableMouseInput = false;

    isLightPopupOpen = false;
    isMaterialPopupOpen = false;

    skybox.setup();
}

void SceneEditor::update() {
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

    skybox.setCamera(*selectedCam);
    skybox.draw();

    if (cameraManager.isFrustumCullingEnabled() && frustumCam) {
        sceneGraph.drawVisibleNodes(*frustumCam);
    }
    else {
        sceneGraph.draw();
    }

    gizmoManager.draw(*selectedCam);

    if (frustumCam && frustumCam != selectedCam) {
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

    ImGui::SetNextWindowSize(ImVec2(400.0f, 0));

    if (isLightPopupOpen) {
        if (!lightModal.isOpen()) {
            cameraManager.disableAllMouseInput();

            if (activeLight) {
                lightModal.openEditLightModal(activeLight);
            }
            else {
                lightModal.openNewLightModal(activeLightType);
            }
        }

        if (activeLight) {
            lightModal.editLight(activeLight);
        }
        else {
            lightModal.createNewLight(sceneGraph);
        }

        if (!lightModal.isOpen()) {
            cameraManager.enableSelectedMouseInput();
            activeLight.reset();
            isLightPopupOpen = false;
        }
    }

    if (isMaterialPopupOpen) {
        if (!materialModal.isOpen()) {
            cameraManager.disableAllMouseInput();
            materialModal.openModal(activeMaterial);
        }

        materialModal.draw();

        if (!materialModal.isOpen()) {
            cameraManager.enableSelectedMouseInput();
            isMaterialPopupOpen = false;
        }
    }

    ImGui::Begin("RayTracing");

    if (ImGui::Button("Render Ray Traced Image")) {
        // Configurer le ray tracer
        rayTracer.setSceneGraph(&sceneGraph);
        rayTracer.setCamera(cameraManager.getSelectedCamera());

        // Lancer le ray tracer pour l'image complète
        rayTracer.render();

        // Variables pour compter les intersections par type
        int sphereCount = 0;
        int cubeCount = 0;
        int tetraCount = 0;

        // Tester des rayons directs vers chaque primitive
        auto allNodes = sceneGraph.getAllNodes();

        for (const auto& node : allNodes) {
            // Vérifier seulement pour les types de primitives qu'on cherche
            if (node->getPrimitiveType() == PrimitiveType::Sphere ||
                node->getPrimitiveType() == PrimitiveType::Cube ||
                node->getPrimitiveType() == PrimitiveType::Tetrahedron) {

                // Créer un rayon depuis la caméra vers le centre de la primitive
                glm::vec3 center = node->getGlobalPosition();
                glm::vec3 rayOrigin = cameraManager.getSelectedCamera()->getGlobalPosition();
                glm::vec3 rayDirection = glm::normalize(center - rayOrigin);
                Ray directRay(rayOrigin, rayDirection);

                // Vérifier l'intersection
                RayHit directHit = rayTracer.intersectScene(directRay, sceneGraph);

                if (directHit.hit && directHit.node) {
                    // Incrémenter le compteur approprié selon le type
                    switch (directHit.node->getPrimitiveType()) {
                    case PrimitiveType::Sphere:
                        sphereCount++;
                        break;
                    case PrimitiveType::Cube:
                        cubeCount++;
                        break;
                    case PrimitiveType::Tetrahedron:
                        tetraCount++;
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        // Afficher le résultat dans une popup
        showIntersectionPopup = true;
        rayIntersectsSphere = (sphereCount > 0);

        // Stocker les compteurs dans des membres temporaires ou les utiliser directement
        // Si vous avez besoin de les utiliser plus tard, ajoutez-les à la classe SceneEditor

        ImGui::OpenPopup("Intersection Result");

        // Stockez ces valeurs dans des variables pour les utiliser dans la popup
        intersectionInfo = "Intersection avec: ";
        bool needComma = false;

        if (sphereCount > 0) {
            intersectionInfo += std::to_string(sphereCount) + " sphere" + (sphereCount > 1 ? "s" : "");
            needComma = true;
        }

        if (cubeCount > 0) {
            if (needComma) intersectionInfo += ", ";
            intersectionInfo += std::to_string(cubeCount) + " cube" + (cubeCount > 1 ? "s" : "");
            needComma = true;
        }

        if (tetraCount > 0) {
            if (needComma) intersectionInfo += ", ";
            intersectionInfo += std::to_string(tetraCount) + " tetrahedron" + (tetraCount > 1 ? "s" : "");
        }
    }

    // Afficher la popup avec le résultat
    if (showIntersectionPopup) {
        if (ImGui::BeginPopup("Intersection Result")) {
            if (rayIntersectsSphere || !intersectionInfo.empty()) {
                ImGui::Text("Intersections detectees!");
                ImGui::Text("%s", intersectionInfo.c_str());
            }
            else {
                ImGui::Text("Aucune intersection.");
            }

            if (ImGui::Button("Fermer")) {
                showIntersectionPopup = false;
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End();


    gizmoManager.drawGui();
    cameraManager.drawGui();
    skybox.drawGui();

    drawInfo(sceneGraph.getSelectedNode());
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

void SceneEditor::onWindowResize(int w, int h) 
{
    gizmoManager.onWindowResize(w, h);
}

void SceneEditor::load(const std::string& path) {
    sceneGraph.addModelNode(path, sceneGraph.getRootNode());
}

void SceneEditor::save(const std::string& path) {

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

    if (ImGui::Button("Set texture"))
    {
        ofFileDialogResult result = ofSystemLoadDialog("Select a texture");
        if (result.bSuccess) 
        {
            node->setTexture(result.getPath());
        }
    }

    ImGui::End();
}

void SceneEditor::drawSceneGraph() {
    drawSceneGraphNode(sceneGraph.getRootNode());
}

void SceneEditor::drawSceneGraphNode(std::shared_ptr<SceneNode> node) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (node == sceneGraph.getRootNode()) {
        if (firstTime) {
            ImGui::SetNextTreeNodeOpen(true);
            firstTime = false;
        }
    }

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

    handleContextMenu(node);

    if (nodeOpen) {
        for (auto& child : node->getChildren()) {
            drawSceneGraphNode(child);
        }

        ImGui::TreePop();
    }
}

void SceneEditor::handleContextMenu(std::shared_ptr<SceneNode> node) {
    if (!node->isSurfaceControl() && ImGui::BeginPopupContextItem()) {
        if (node != sceneGraph.getRootNode()) {
            if (ImGui::Selectable("Delete")) {
                if (node->hasLight()) {
                    node->getLight()->disable();
                }

                sceneGraph.deleteNode(node);
            }
            ImGui::Separator();
        }

        if (node->hasLight()) {
            if (ImGui::Selectable("Edit Light")) {
                activeLight = node->getLight();
                isLightPopupOpen = true;
            }
            ImGui::Separator();
        }

        if (node->hasMaterial() && !node->hasLight()) {
            if (ImGui::Selectable("Edit Material")) {
                activeMaterial = node->getMaterial();
                isMaterialPopupOpen = true;
            }
            ImGui::Separator();
        }

        if (ImGui::Selectable("New Empty")) newEmptyObject("Empty", node);

        ImGui::Separator();

        if (ImGui::Selectable("New Sphere")) newPrimitiveObject(PrimitiveType::Sphere, "Sphere", node);
        if (ImGui::Selectable("New Pyramid")) newPrimitiveObject(PrimitiveType::Tetrahedron, "Pyramid", node);
        if (ImGui::Selectable("New Cube")) newPrimitiveObject(PrimitiveType::Cube, "Cube", node);
        if (ImGui::Selectable("New Cylinder")) newPrimitiveObject(PrimitiveType::Cylinder, "Cylinder", node);
        if (ImGui::Selectable("New Cone")) newPrimitiveObject(PrimitiveType::Cone, "Cone", node);
        if (ImGui::Selectable("New Bezier Surface")) newSurfaceObject("Parametric Surface", node);

        ImGui::Separator();

        if (ImGui::Selectable("New Ambient Light")) {
            activeLightType = LightModal::LightType::Ambient;
            isLightPopupOpen = true;
        }
        if (ImGui::Selectable("New Point Light")) {
            activeLightType = LightModal::LightType::Point;
            isLightPopupOpen = true;
        }
        if (ImGui::Selectable("New Directional Light")) {
            activeLightType = LightModal::LightType::Directional;
            isLightPopupOpen = true;
        }
        if (ImGui::Selectable("New Spot Light")) {
            activeLightType = LightModal::LightType::Spot;
            isLightPopupOpen = true;
        }

        ImGui::EndPopup();
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

void SceneEditor::newSurfaceObject(const std::string& name, std::shared_ptr<SceneNode> parent) {
    sceneGraph.addSurfaceNode(name, parent);
    justAddedNode = true;
}
