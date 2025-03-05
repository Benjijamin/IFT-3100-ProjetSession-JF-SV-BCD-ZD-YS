#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);

    gui.setup();

    ImGui::StyleColorsClassic();
    ofSetBackgroundColor(ofColor(60, 60, 60));

    imageEditor.setup();
    modelEditor.setup();
    sceneEditor.setup();
    assetBrowser.setup();
    menuBar.setup();
    dessinVectoriel.setup();

    currentEditor = nullptr;

    dessinVectoriel.onNewDrawing = std::bind(&ofApp::handleNewDrawing, this);

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

    if (!dessinVectoriel.isActive()) {
        dessinVectoriel.drawInit();
    }

    assetBrowser.drawGui();
    menuBar.drawGui();

    gui.end();
}

void ofApp::exit() {
    if (currentEditor) {
        currentEditor->exit();
    }
    assetBrowser.exit();
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

    //TODO Mettre dans la barre d'onglets
    if (key == 'o')
    {
        if (currentEditor) {
            currentEditor->exit();
        }

        currentEditor = &sceneEditor;

        currentEditor->setup();
    }
}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseDragged(x, y, button);
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mousePressed(x, y, button);
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseReleased(x, y, button);
    }
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

void ofApp::handleNewDrawing() {
    currentEditor = &dessinVectoriel;
}

void ofApp::handleAssetAddition() {
    std::string lastAsset = assetBrowser.getLastAssetPath();

    if (assetBrowser.isModelAsset(lastAsset)) {
        modelEditor.load(lastAsset);
    }
}

void ofApp::handleAssetRemoval() {

}

void ofApp::handleAssetSelection() {
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
