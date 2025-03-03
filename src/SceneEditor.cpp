#include "SceneEditor.h"

void SceneEditor::setup() 
{
    sceneName = "Scene";
	sceneGraph.setup();
    justAddedNode = false;
}

void SceneEditor::update() 
{
	sceneGraph.update();
}

void SceneEditor::draw() 
{
    ofEnableDepthTest();
    ofEnableLighting();

    camera.begin();

    light.enable();
    material.begin();

    sceneGraph.draw();

    material.end();
    light.disable();

    camera.end();

    ofDisableDepthTest();
    ofDisableLighting();
}

void SceneEditor::drawGui() 
{
    ImGui::Begin(sceneName);

    //Si on click dans le vide on deselectionne la node courante
    if (ImGui::IsMouseClicked(0) && ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
    {
        sceneGraph.setSelectedNode(sceneGraph.getRootNode());
    }

    auto selected = sceneGraph.getSelectedNode();

    //Ajouter une node vide sous la node courante
    if (ImGui::Button("Add empty")) 
    {
        newEmptyObject(selected);
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete")) 
    {
        if (selected != sceneGraph.getRootNode()) 
        {
            sceneGraph.deleteNode(selected);
            sceneGraph.setSelectedNode(sceneGraph.getRootNode());
        }
    }

    drawSceneGraph();

    ImGui::End();

    //On affiche les infos de la node courante
    if (selected != sceneGraph.getRootNode()) {
        drawInfo(selected);
    }
}

void SceneEditor::drawInfo(std::shared_ptr<SceneNode> node) 
{
    ImGui::Begin("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);

    static char name[255] = "";
    strncpy(name, node->getName().c_str(), sizeof(name) - 1);
    ImGui::InputText("##NameInput", name, IM_ARRAYSIZE(name));
    if (ImGui::IsItemDeactivatedAfterEdit()) 
    {
        std::string newName = name;
        sceneGraph.editNodeName(node, newName);
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    //Position
    ImGui::Text("Position");
    glm::vec3 position = node->getPosition();

    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::InputFloat("##PosXInput", &position.x);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setPosition(position);

    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::InputFloat("##PosYInput", &position.y);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setPosition(position);

    ImGui::Text("z: ");
    ImGui::SameLine();
    ImGui::InputFloat("##PosZInput", &position.z);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setPosition(position);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    //Scale
    ImGui::Text("Scale");
    glm::vec3 scale = node->getScale();

    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::InputFloat("##ScaleXInput", &scale.x);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setScale(scale);

    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::InputFloat("##ScaleYInput", &scale.y);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setScale(scale);

    ImGui::Text("z: ");
    ImGui::SameLine();
    ImGui::InputFloat("##ScaleZInput", &scale.z);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setScale(scale);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    //Rotation
    ImGui::Text("Rotation");
    glm::vec3 rotation = node->getOrientationEuler();

    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::InputFloat("##RotationXInput", &rotation.x);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setOrientation(rotation);

    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::InputFloat("##RotationYInput", &rotation.y);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setOrientation(rotation);

    ImGui::Text("z: ");
    ImGui::SameLine();
    ImGui::InputFloat("##RotationZInput", &rotation.z);
    if (ImGui::IsItemDeactivatedAfterEdit()) node->setOrientation(rotation);

    ImGui::End();
}

void SceneEditor::drawSceneGraph() 
{
    for (auto child : sceneGraph.getRootNode()->getChildren())
    {
        drawSceneGraphNode(child);
    }
}

void SceneEditor::drawSceneGraphNode(std::shared_ptr<SceneNode> node) 
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (node == sceneGraph.getSelectedNode()) 
    { 
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
        
        //On ouvre le parent automatiquement apres avoir ajoute une node sinon ca fait bizarre
        if (justAddedNode)
        {
            ImGui::SetNextTreeNodeOpen(true);
            justAddedNode = false;
        }
    }

    bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), nodeFlags);
    nodeDragDropBehaviour(node);

    if (ImGui::IsItemClicked())
    {
        sceneGraph.setSelectedNode(node);
    }

    //Opens on right click
    if (ImGui::BeginPopupContextItem()) 
    {
        newObjectMenu(node);
        ImGui::EndPopup();
    }

    if (nodeOpen) 
    {
        for (auto child : node->getChildren()) 
        {
            drawSceneGraphNode(child);
        }

        ImGui::TreePop();
    }
}

void SceneEditor::newObjectMenu(std::shared_ptr<SceneNode> node) 
{
    if (ImGui::Selectable("New Empty")) newEmptyObject(node);

    if (ImGui::Selectable("New Sphere")) newPrimitiveObject(PrimitiveType::Sphere, "Sphere", node);

    if (ImGui::Selectable("New Cube")) newPrimitiveObject(PrimitiveType::Cube, "Cube", node);

    if (ImGui::Selectable("New Cylinder")) newPrimitiveObject(PrimitiveType::Cylinder, "Cylinder", node);

    if (ImGui::Selectable("New Cone")) newPrimitiveObject(PrimitiveType::Cone, "Cone", node);

    if (ImGui::Selectable("New Camera")) newCameraObject(node);
}

void SceneEditor::nodeDragDropBehaviour(std::shared_ptr<SceneNode> node) 
{
    //Source
    if (ImGui::BeginDragDropSource()) {
        SceneNode* nodePointer = node.get();
        ImGui::SetDragDropPayload("MOVE_NODE", &nodePointer, sizeof(SceneNode*));
        ImGui::Text("%s", node->getName().c_str());
        ImGui::EndDragDropSource();
    }

    //Target
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MOVE_NODE")) {
            SceneNode* sourceNode = *(SceneNode**)payload->Data;
            sceneGraph.transferNode(sourceNode->shared_from_this(), node);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET")) {
            const char* asset = (const char*)payload->Data;
            std::string assetPath(asset);

            ofLog() << "Loading " + assetPath + " into " + node->getName();
            sceneGraph.loadAsset(node, assetPath);
        }
        ImGui::EndDragDropTarget();
    }
}

void SceneEditor::newEmptyObject(std::shared_ptr<SceneNode> parent) 
{
    sceneGraph.addEmptyNode("New Object", parent);
    justAddedNode = true;
}

void SceneEditor::newPrimitiveObject(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent) 
{
    sceneGraph.addPrimitiveNode(primitiveType, name, parent);
    justAddedNode = true;
}

void SceneEditor::newCameraObject(std::shared_ptr<SceneNode> parent) 
{
    //TODO
    justAddedNode = true;
}

void SceneEditor::exit() {}

void SceneEditor::mouseDragged(int x, int y, int button) {}

void SceneEditor::mousePressed(int x, int y, int button) {}

void SceneEditor::mouseReleased(int x, int y, int button) {}

void SceneEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {}

void SceneEditor::load(const std::string& path) {}

void SceneEditor::unload(const std::string& path) {}

void SceneEditor::save(const std::string& path) {}