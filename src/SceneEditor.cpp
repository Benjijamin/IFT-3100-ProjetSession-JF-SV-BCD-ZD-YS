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
        auto emptyNode = std::make_shared<SceneNode>(sceneGraph.generateUniqueName("New Object"));

        if (selected != sceneGraph.getRootNode()) 
        {
            selected->addChild(emptyNode);
            justAddedNode = true;
        }
        else 
        {
            sceneGraph.getRootNode()->addChild(emptyNode);
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
    ImGui::InputText("", name, IM_ARRAYSIZE(name));
    if (ImGui::IsItemDeactivatedAfterEdit()) 
    {
        std::string newName = name;
        sceneGraph.editNodeName(node, newName);
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Position");
    glm::vec3 position = node->getPosition();
    ImGui::Text("x: %f", position.x);
    ImGui::Text("y: %f", position.y);
    ImGui::Text("z: %f", position.z);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Scale");
    glm::vec3 scale = node->getScale();
    ImGui::Text("x: %f", scale.x);
    ImGui::Text("y: %f", scale.y);
    ImGui::Text("z: %f", scale.z);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Rotation");
    glm::vec3 rotation = node->getOrientationEuler();
    ImGui::Text("x: %f", rotation.x);
    ImGui::Text("y: %f", rotation.y);
    ImGui::Text("z: %f", rotation.z);

    //Ajouter autre infos et controls

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
        }
    }

    bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), nodeFlags);


    if (ImGui::IsItemClicked())
    {
        sceneGraph.setSelectedNode(node);
    }

    if (nodeOpen) 
    {
        for (auto child : node->getChildren()) 
        {
            drawSceneGraphNode(child);
        }

        ImGui::TreePop();
    }

    justAddedNode = false;
}

void SceneEditor::exit() {}

void SceneEditor::mouseDragged(int x, int y, int button) {}

void SceneEditor::mousePressed(int x, int y, int button) {}

void SceneEditor::mouseReleased(int x, int y, int button) {}

void SceneEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {}

void SceneEditor::load(const std::string& path) {}

void SceneEditor::unload(const std::string& path) 
{
    //delete all nodes
}

void SceneEditor::save(const std::string& path) {}