#pragma once

#include "ofMain.h"
//#include "ofxImGui.h"
//#include "Editor.h"
//#include "ImageEditor.h"
//#include "SceneEditor.h"
//#include "DessinVectoriel.h"
//#include "AssetBrowser.h"
//#include "MenuBar.h"
//#include "DynamicCursor.h"
#include "renderer.h"

// code original, a remettre en place avant merge
/*
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
*/

class ofApp : public ofBaseApp {
public:

    Renderer renderer;

    float time_current;
    float time_last;
    float time_elapsed;

    bool is_key_press_up;
    bool is_key_press_down;
    bool is_key_press_left;
    bool is_key_press_right;

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    /*
    ofLight dirLight;           // Lumière directionnelle

    // Shaders
    ofShader lambertShader;
    ofShader phongShader;

    ofSpherePrimitive sphere;   // Objet à illuminer

    // Material
    ofMaterial material;        // Matériau appliqué

    // Direction lumiere
    ofVec3f lightDirection;     // Direction de la lumière
    */
};