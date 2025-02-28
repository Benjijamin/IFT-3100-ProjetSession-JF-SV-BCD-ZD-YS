#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);

    gui.setup();

    ImGui::StyleColorsClassic();
    ofSetBackgroundColor(ofColor(60, 60, 60));

    imageEditor.setup();
    modelEditor.setup();
    assetBrowser.setup();
    menuBar.setup();
    dessinVectoriel.setup();

    currentEditor = nullptr;

    assetBrowser.onAssetAddition = std::bind(&ofApp::handleAssetAddition, this);
    assetBrowser.onAssetRemoval = std::bind(&ofApp::handleAssetRemoval, this);
    assetBrowser.onAssetSelection = std::bind(&ofApp::handleAssetSelection, this);
}

void ofApp::update() {
    if (currentEditor) {
        currentEditor->update();
    }
}

void ofApp::draw() {
    if (currentEditor) {
        currentEditor->draw();
    }

    gui.begin();

    if (currentEditor) {
        currentEditor->drawGui();
    }

    dessinVectoriel.drawGui();
    dessinVectoriel.isActive() ? dessinVectoriel.draw() : assetBrowser.drawGui();
    menuBar.drawGui();

    gui.end();
}

void ofApp::exit() {
    if (currentEditor) {
        currentEditor->exit();
    }
    assetBrowser.exit();
    dessinVectoriel.exit();
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseDragged(x, y, button);
    }
    else if (dessinVectoriel.isActive())
        dessinVectoriel.mouseDragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mousePressed(x, y, button);
    }
    else if (dessinVectoriel.isActive())
        dessinVectoriel.mousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseReleased(x, y, button);
    }
    else if (dessinVectoriel.isActive())
        dessinVectoriel.mouseReleased(x, y, button);
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (currentEditor) {
        currentEditor->mouseScrolled(x, y, scrollX, scrollY);
    }
}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::switchEditor() {
    std::string selectedAsset = assetBrowser.getSelectedAssetPath();

    if (!selectedAsset.empty()) {
        if (currentEditor) {
            currentEditor->exit();
        }

        if (assetBrowser.isImageAsset(selectedAsset)) {
            currentEditor = &imageEditor;
            currentEditor->load(selectedAsset);
        }
        else if (assetBrowser.isModelAsset(selectedAsset)) {
            currentEditor = &modelEditor;
        }
    }
}

void ofApp::handleAssetAddition() {
    std::string lastAsset = assetBrowser.getLastAssetPath();

    if (assetBrowser.isModelAsset(lastAsset)) {
        modelEditor.load(lastAsset);
    }
}

void ofApp::handleAssetRemoval() {
    std::string selectedAsset = assetBrowser.getSelectedAssetPath();

    if (currentEditor) {
        currentEditor->unload(selectedAsset);
    }
}

void ofApp::handleAssetSelection() {
    switchEditor();
}
