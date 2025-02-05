#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    gui.setup();

    imageEditor.setup();
    modelEditor.setup();
    screenCapture.setup();

    currentEditor = EditorType::Image;

    assetBrowser.onAssetSelection = std::bind(&ofApp::handleAssetSelection, this);
    assetBrowser.onAssetDeletion = std::bind(&ofApp::handleAssetDeletion, this);
}

void ofApp::update() {
    if (currentEditor == EditorType::Image) {
        imageEditor.update();
    }
    else if (currentEditor == EditorType::Model) {
        modelEditor.update();
    }
}

void ofApp::draw() {
    screenCapture.draw();

    gui.begin();

    if (currentEditor == EditorType::Image) {
        imageEditor.draw();
    }
    else if (currentEditor == EditorType::Model) {
        modelEditor.draw();
    }

    assetBrowser.drawGui();
    screenCapture.drawGui();

    if (currentEditor == EditorType::Image) {
        imageEditor.drawGui();
    }
    else if (currentEditor == EditorType::Model) {
        modelEditor.drawGui();
    }

    gui.end();
}

void ofApp::exit() {
    imageEditor.exit();
    modelEditor.exit();
    screenCapture.exit();
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {
    if (currentEditor == EditorType::Image) {
        imageEditor.mouseDragged(x, y, button);
    }
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (currentEditor == EditorType::Image) {
        imageEditor.mouseScrolled(x, y, scrollX, scrollY);
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    if (currentEditor == EditorType::Image) {
        imageEditor.mousePressed(x, y, button);
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (currentEditor == EditorType::Image) {
        imageEditor.mouseReleased(x, y, button);
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
        if (assetBrowser.isImageAsset(selectedAsset)) {
            currentEditor = EditorType::Image;
            imageEditor.loadImage(selectedAsset);
        }
        else if (assetBrowser.isModelAsset(selectedAsset)) {
            currentEditor = EditorType::Model;
            modelEditor.loadModel(selectedAsset);
        }
    }
}

void ofApp::handleAssetSelection() {
    switchEditor();
}

void ofApp::handleAssetDeletion() {
    if (currentEditor == EditorType::Image) {
        imageEditor.exit();
    }
    else if (currentEditor == EditorType::Model) {
        modelEditor.exit();
    }
}
