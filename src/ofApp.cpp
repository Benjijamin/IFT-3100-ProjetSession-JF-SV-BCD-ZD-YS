#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);

    gui.setup();

    ImGui::StyleColorsClassic();
    ofSetBackgroundColor(ofColor(60, 60, 60));

    renderer.setup();
    materials.setup();

    menuBar.setRenderer(&renderer);
    menuBar.setMaterial(&materials);
    menuBar.setup();

    imageEditor.setup();
    sceneEditor.setup();
    assetBrowser.setup();
    dessinVectoriel.setup();
    dynamicCursor.setup();

    currentEditor = nullptr;

    menuBar.onNewDrawing = std::bind(&ofApp::handleNewDrawing, this);
    menuBar.tonemapping = &tonemapping;

    assetBrowser.onAssetAddition = std::bind(&ofApp::handleAssetAddition, this);
    assetBrowser.onAssetRemoval = std::bind(&ofApp::handleAssetRemoval, this);
    assetBrowser.onAssetSelection = std::bind(&ofApp::handleAssetSelection, this);
    assetBrowser.onAssetSave = std::bind(&ofApp::handleAssetSave, this, std::placeholders::_1);

    ppFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    acesShader.load("tonemapping_aces_330_vs.glsl", "tonemapping_aces_330_fs.glsl");
}

void ofApp::update() {
    if (currentEditor) {
        currentEditor->update();
    }
}

void ofApp::draw() 
{
    ppFbo.begin();
    ofClear(0);
    if (currentEditor) {
        currentEditor->draw();
    }
    ppFbo.end();

    if (tonemapping) {
        acesShader.begin();
        ppFbo.draw(0, 0);
        acesShader.end();
    }
    else 
    {
        ppFbo.draw(0, 0);
    }

    gui.begin();

    if (currentEditor) {
        currentEditor->drawGui();
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
    menuBar.exit();
}

void ofApp::keyPressed(int key) {
    if (key == 'w' || key == 'W') {
        dynamicCursor.setCursorType(DynamicCursor::FRONT);
    }

    if (key == 'a' || key == 'A') {
        dynamicCursor.setCursorType(DynamicCursor::LEFT);
    }

    if (key == 'd' || key == 'd') {
        dynamicCursor.setCursorType(DynamicCursor::RIGHT);
    }

    if (key == 's' || key == 's') {
        dynamicCursor.setCursorType(DynamicCursor::BACK);
    }
}

void ofApp::keyReleased(int key) {
dynamicCursor.resetCursor();
    
}

void ofApp::mouseMoved(int x, int y) {

    dynamicCursor.update(x, y);
}



void ofApp::mouseDragged(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseDragged(x, y, button);
    }
    dynamicCursor.setCursorType(DynamicCursor::RESIZE);
}

void ofApp::mousePressed(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mousePressed(x, y, button);
    }
    dynamicCursor.setCursorType(DynamicCursor::CROSSHAIR);
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (currentEditor) {
        currentEditor->mouseReleased(x, y, button);
    }
    dynamicCursor.resetCursor();
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (currentEditor) {
        currentEditor->mouseScrolled(x, y, scrollX, scrollY);
    }
    dynamicCursor.setCursorType(DynamicCursor::SCROLL);
}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {
    ppFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    sceneEditor.onWindowResize(w, h);
}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::handleNewDrawing() {
    dessinVectoriel.begin();

    currentEditor = &dessinVectoriel;
}

void ofApp::handleAssetAddition() {
    std::string lastAsset = assetBrowser.getLastAssetPath();

    if (assetBrowser.isModelAsset(lastAsset)) {
        sceneEditor.load(lastAsset);
    }
}

void ofApp::handleAssetRemoval() {
    currentEditor = nullptr;
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
            currentEditor = &sceneEditor;
        }
    }
}

void ofApp::handleAssetSave(const std::string& path) {
    currentEditor->save(path);
}
