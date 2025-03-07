#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"
#include "ImageEditor.h"
#include "SceneEditor.h"
#include "DessinVectoriel.h"
#include "AssetBrowser.h"
#include "MenuBar.h"
#include "DynamicCursor.h"



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
    void handleNewDrawing();
    void handleAssetAddition();
    void handleAssetRemoval();
    void handleAssetSelection();
    void handleAssetSave(const std::string& path);

    ofxImGui::Gui gui;

    ImageEditor imageEditor;
    SceneEditor sceneEditor;
    DessinVectoriel dessinVectoriel;
    AssetBrowser assetBrowser;
    MenuBar menuBar;
    DynamicCursor dynamicCursor;

    Editor* currentEditor;
};
