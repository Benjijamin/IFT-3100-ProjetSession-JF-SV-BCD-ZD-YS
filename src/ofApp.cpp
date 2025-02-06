#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    gui.setup();

    assetBrowser.setup();
    screenCapture.setup();

    currentEditor = nullptr;

    assetBrowser.onAssetSelection = std::bind(&ofApp::handleAssetSelection, this);
    assetBrowser.onAssetDeletion = std::bind(&ofApp::handleAssetDeletion, this);
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

    screenCapture.draw();

    gui.begin();

    if (currentEditor) {
        currentEditor->drawGui();
    }

    assetBrowser.drawGui();
    screenCapture.drawGui();

    gui.end();
}

void ofApp::exit() {
    if (currentEditor) {
        currentEditor->exit();
    }

    assetBrowser.exit();
    screenCapture.exit();
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {
    if (key == 90) {
        currentEditor.reset();
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

void ofApp::switchEditor() {
    std::string selectedAsset = assetBrowser.getSelectedAssetPath();
    if (!selectedAsset.empty()) {
        if (currentEditor) {
            currentEditor->exit();
        }

        if (assetBrowser.isImageAsset(selectedAsset)) {
            currentEditor = std::make_unique<ImageEditor>();
        }
        else if (assetBrowser.isModelAsset(selectedAsset)) {
            currentEditor = std::make_unique<ModelEditor>();
        }

        if (currentEditor) {
            currentEditor->setup();
            currentEditor->load(selectedAsset);
        }
    }
}

void ofApp::handleAssetSelection() {
    switchEditor();
}

void ofApp::handleAssetDeletion() {
    if (currentEditor) {
        currentEditor->exit();
    }
    currentEditor.reset();
}
