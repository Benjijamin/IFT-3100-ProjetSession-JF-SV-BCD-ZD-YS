#include "MenuBar.h"

void MenuBar::setup() {
    screenCapture.setup();
}

void MenuBar::update() {
    screenCapture.update();
}

void MenuBar::draw() {
    screenCapture.draw();
}

void MenuBar::drawGui() {
    if (ImGui::BeginMainMenuBar()) {
        drawFileMenu();
        drawSettingsMenu();
        drawHelpMenu();
        ImGui::EndMainMenuBar();
    }
    screenCapture.drawGui();
}

void MenuBar::exit() {
    screenCapture.exit();
}

void MenuBar::drawFileMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Capture Screenshot")) {
            screenCapture.captureScreenshot();
        }
        if (ImGui::MenuItem("Exit")) {
            ofExit();
        }
        ImGui::EndMenu();
    }
}

void MenuBar::drawSettingsMenu() {
    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::MenuItem("Classic Theme")) {
            ImGui::StyleColorsClassic();
            ofSetBackgroundColor(ofColor(60, 60, 60));
        }

        if (ImGui::MenuItem("Light Theme")) {
            ImGui::StyleColorsLight();
            ofSetBackgroundColor(ofColor(230, 242, 255));
        }

        if (ImGui::MenuItem("Dark Theme")) {
            ImGui::StyleColorsDark();
            ofSetBackgroundColor(ofColor(32, 32, 32));
        }

        if (ImGui::MenuItem("Fullscreen")) {
            ofToggleFullscreen();
        }

        if (ImGui::MenuItem("Windowed")) {
            ofSetFullscreen(false);
        }

        ImGui::EndMenu();
    }
}

void MenuBar::drawHelpMenu() {
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("Documentation")) {
            ofLaunchBrowser("https://openframeworks.cc/documentation/");
        }
        ImGui::EndMenu();
    }
}
