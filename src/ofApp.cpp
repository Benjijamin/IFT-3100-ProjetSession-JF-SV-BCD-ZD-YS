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
			if (ImGui::MenuItem("1"))
			{
				ofLog() << "test1";
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