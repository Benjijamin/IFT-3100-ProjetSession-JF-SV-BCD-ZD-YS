#include "ofApp.h"
#include "ofxImGui.h"

ofxImGui::Gui gui;

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.begin();
	
	menuBar();

	colorPicker();

	gui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::menuBar() 
{
	//MenuBar

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::MenuItem("Color Picker"))
			{
				showColorPicker = true;
			}

			if (ImGui::MenuItem("2"))
			{
				ofLog() << "test2";
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ofApp::colorPicker() 
{
	if (showColorPicker)
	{
		ImGui::Begin("ColorPicker", &showColorPicker, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("RGB")) colorPickerDisplay = 1;
		ImGui::SameLine();
		if (ImGui::Button("HSB")) colorPickerDisplay = 2;

		switch (colorPickerDisplay)
		{
		default:
			break;
		case 1:
			ImGui::ColorPicker4("RGB", pickedColor, ImGuiColorEditFlags_DisplayRGB);
			break;
		case 2:
			ImGui::ColorPicker4("HSB", pickedColor, ImGuiColorEditFlags_DisplayHSV);
			break;
		}

		ImGui::End();
	}
}