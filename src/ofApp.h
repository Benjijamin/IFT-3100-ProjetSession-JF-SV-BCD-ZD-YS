#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "ImageEditor.h"
#include "ModelEditor.h"
#include "AssetBrowser.h"
#include "ScreenCapture.h"
#include "MenuBar.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);  
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    void switchEditor();
    void handleAssetAddition();
    void handleAssetRemoval();
    void handleAssetSelection();

    ofxImGui::Gui gui;

    ImageEditor imageEditor;
    ModelEditor modelEditor;
    AssetBrowser assetBrowser;
    ScreenCapture screenCapture;
    MenuBar menuBar;

    Editor* currentEditor;
};
