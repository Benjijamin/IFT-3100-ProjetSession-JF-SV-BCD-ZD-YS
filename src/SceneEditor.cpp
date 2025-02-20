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

    //draw all scene graph nodes starting from children of root
    drawSceneGraph();

    ImGui::End();

    //draw selected item info
    if (sceneGraph.getSelectedIndex() != -1) {
        ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::Text("ABC");
        ImGui::End();
    }
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
    if (justAddedNode && index == sceneGraph.getSelectedIndex()) ImGui::SetNextTreeNodeOpen(true);

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