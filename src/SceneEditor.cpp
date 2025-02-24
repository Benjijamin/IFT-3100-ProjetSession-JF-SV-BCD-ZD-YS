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
        sceneGraph.setSelectedIndex(-1);
    }

    //Ajouter une node vide sous la node courante
    if (ImGui::Button("+")) 
    {
        if (sceneGraph.getSelectedIndex() != -1) 
        {
            sceneGraph.addEmptyNode("test", sceneGraph.getSelectedNode());
            justAddedNode = true;
        }
        else 
        {
            sceneGraph.addEmptyNode("noParent", sceneGraph.getRootNode());
        }
    }

    //On dessine le graphe
    drawSceneGraph();

    ImGui::End();

    //On affiche les infos de la node courante
    if (sceneGraph.getSelectedIndex() != -1) {
        drawInfo();
    }
}

void SceneEditor::drawInfo() 
{
    ImGui::Begin("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);

    SceneNode& selected = sceneGraph.getSelectedNode();

    ImGui::Text(selected.getName().c_str());

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Position");
    glm::vec3 position = selected.getPosition();
    ImGui::Text("x: %f", position.x);
    ImGui::Text("y: %f", position.y);
    ImGui::Text("z: %f", position.z);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Scale");
    glm::vec3 scale = selected.getScale();
    ImGui::Text("x: %f", scale.x);
    ImGui::Text("y: %f", scale.y);
    ImGui::Text("z: %f", scale.z);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Rotation");
    glm::vec3 rotation = selected.getOrientationEuler();
    ImGui::Text("x: %f", rotation.x);
    ImGui::Text("y: %f", rotation.y);
    ImGui::Text("z: %f", rotation.z);

    ImGui::End();
}

void SceneEditor::drawSceneGraph() 
{
    for (int child : sceneGraph.getChildrenOf(0)) 
    {
        drawSceneGraphNode(child);
    }
}

void SceneEditor::drawSceneGraphNode(int index) 
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (index == sceneGraph.getSelectedIndex()) nodeFlags |= ImGuiTreeNodeFlags_Selected;

    //On ouvre le parent automatiquement apres avoir ajoute une node sinon ca fait bizarre
    if (justAddedNode && index == sceneGraph.getSelectedIndex()) 
    {
        ImGui::SetNextTreeNodeOpen(true);
    }

    bool nodeOpen = ImGui::TreeNodeEx(sceneGraph.getNode(index).getName().c_str(), nodeFlags);


    if (ImGui::IsItemClicked())
    {
        sceneGraph.setSelectedIndex(index);
    }

    if (nodeOpen) 
    {
        for (int child : sceneGraph.getChildrenOf(index)) 
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

void SceneEditor::save(const std::string& path) {}