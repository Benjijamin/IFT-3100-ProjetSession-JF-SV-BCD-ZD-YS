#include "SceneEditor.h"

void SceneEditor::setup() 
{
	sceneGraph.setup();
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
	float frameHeight = ImGui::GetFrameHeight();
	ImGui::SetNextWindowPos(ImVec2(0, frameHeight));

	sceneGraph.drawGui();
}

void SceneEditor::exit() {}

void SceneEditor::mouseDragged(int x, int y, int button) {}

void SceneEditor::mousePressed(int x, int y, int button) {}

void SceneEditor::mouseReleased(int x, int y, int button) {}

void SceneEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {}

void SceneEditor::load(const std::string& path) {}

void SceneEditor::save(const std::string& path) {}