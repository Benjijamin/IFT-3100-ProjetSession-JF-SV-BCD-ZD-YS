#include "ofApp.h"

// code original, a remettre en place avant merge

void ofApp::setup() {
    ofSetFrameRate(60);

    gui.setup();

    ImGui::StyleColorsClassic();
    ofSetBackgroundColor(ofColor(60, 60, 60));

    renderer.setup();
    //materials.setup();
    menuBar.setup();

    menuBar.setRenderer(&renderer);
    menuBar.setMaterial(&materials);
    //renderer.setMaterial(&materials);

    imageEditor.setup();
    sceneEditor.setup();
    assetBrowser.setup();
    dessinVectoriel.setup();
    dynamicCursor.setup();

    currentEditor = nullptr;

    menuBar.onNewDrawing = std::bind(&ofApp::handleNewDrawing, this);

    materialNames = materials.list();

    assetBrowser.onAssetAddition = std::bind(&ofApp::handleAssetAddition, this);
    assetBrowser.onAssetRemoval = std::bind(&ofApp::handleAssetRemoval, this);
    assetBrowser.onAssetSelection = std::bind(&ofApp::handleAssetSelection, this);
    assetBrowser.onAssetSave = std::bind(&ofApp::handleAssetSave, this, std::placeholders::_1);
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



/*
// IFT3100H24_ShaderClassic/application.cpp
// Classe principale de l'application.

void ofApp::setup() {
    ofLog() << "<app::setup>";

    materials.setup(); // Matériau par défaut
    renderer.setMaterialsLibrary(&materials);
    materialNames = materials.getMaterialNames();

    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;

    renderer.setup();
}

void ofApp::update() {
    ofSetWindowTitle("Renderer : " + renderer.shader_name + " (1-6 ↑↓←→ r)");

    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    if (is_key_press_up)
        renderer.offset_z += renderer.delta_z * time_elapsed;
    if (is_key_press_down)
        renderer.offset_z -= renderer.delta_z * time_elapsed;
    if (is_key_press_left)
        renderer.offset_x += renderer.delta_x * time_elapsed;
    if (is_key_press_right)
        renderer.offset_x -= renderer.delta_x * time_elapsed;

    renderer.update();
}

void ofApp::draw() {
    renderer.draw();
}

void ofApp::mouseReleased(int x, int y, int button) {
    renderer.reset();
}

void ofApp::keyPressed(int key) {
    switch (key) {
    case OF_KEY_LEFT: // touche ←
        is_key_press_left = true;
        break;

    case OF_KEY_UP: // touche ↑
        is_key_press_up = true;
        break;

    case OF_KEY_RIGHT: // touche →
        is_key_press_right = true;
        break;

    case OF_KEY_DOWN: // touche ↓
        is_key_press_down = true;
        break;

    default:
        break;
    }
}

void ofApp::keyReleased(int key) {
    switch (key) {
    case 49: // touche 1
        renderer.shader_active = ShaderType::color_fill;
        ofLog() << "<shader: color fill>";
        break;

    case 50: // touche 2
        renderer.shader_active = ShaderType::lambert;
        ofLog() << "<shader: lambert>";
        break;

    case 51: // touche 3
        renderer.shader_active = ShaderType::gouraud;
        ofLog() << "<shader: gouraud>";
        break;

    case 52: // touche 4
        renderer.shader_active = ShaderType::phong;
        ofLog() << "<shader: phong>";
        break;

    case 53: // touche 5
        renderer.shader_active = ShaderType::blinn_phong;
        ofLog() << "<shader: blinn-phong>";
        break;

    case 54: // touche 6
        renderer.shader_active = ShaderType::toon;
        ofLog() << "<shader: toon>";
        break;

    case 55: // touche 7
        renderer.is_active_light_ambient = !renderer.is_active_light_ambient;
        ofLog() <<"<Light ambient: " << renderer.is_active_light_ambient << ">";
        break;

    case 56: // touche 8
        renderer.is_active_light_directional = !renderer.is_active_light_directional;
        ofLog() << "<light directional: " << renderer.is_active_light_directional << ">";
        break;

    case 57: // touche 9
        renderer.is_active_light_point = !renderer.is_active_light_point;
        ofLog() << "<light point: " << renderer.is_active_light_point << ">";
        break;

    case 48: // touche 0
        renderer.is_active_light_spot = !renderer.is_active_light_spot;
        ofLog() << "<light spot: " << renderer.is_active_light_spot << ">";
        break;

    case 114: // touche r
        renderer.reset();
        break;

    case OF_KEY_LEFT: // touche ←
        is_key_press_left = false;
        break;

    case OF_KEY_UP: // touche ↑
        is_key_press_up = false;
        break;

    case OF_KEY_RIGHT: // touche →
        is_key_press_right = false;
        break;

    case OF_KEY_DOWN: // touche ↓
        is_key_press_down = false;
        break;

    default:
        break;
    }
}

void ofApp::windowResized(int w, int h) {
    renderer.reset();

    ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
}

void ofApp::exit() {
    ofLog() << "<app::exit>";
}
*/