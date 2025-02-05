#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);

    imageEditor.setup();
    gui.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
    imageEditor.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    gui.begin();

    imageEditor.draw();
    imageEditor.drawGui();

    createFileWindow();

    gui.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
    imageEditor.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    imageEditor.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    imageEditor.mouseScrolled(x, y, scrollX, scrollY);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    imageEditor.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    imageEditor.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::createFileWindow() {
    if (ImGui::Begin("File")) {
        if (ImGui::Button("Open Image")) {
            openImage();
        }

        if (ImGui::Button("Save Image")) {
            saveImage();
        }
    }

    ImGui::End();
}

void ofApp::openImage() {
    ofFileDialogResult result = ofSystemLoadDialog("Select an image file");
    if (result.bSuccess) {
        imageEditor.loadImage(result.getPath());
    }
}

void ofApp::saveImage() {
    ofFileDialogResult result = ofSystemSaveDialog("image.png", "Save your image file");
    if (result.bSuccess) {
        imageEditor.saveImage(result.getPath());
    }
}
