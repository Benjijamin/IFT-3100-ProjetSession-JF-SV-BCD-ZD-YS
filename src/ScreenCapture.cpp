#include "ScreenCapture.h"

void ScreenCapture::setup() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    screenshotTexture = 0;
    showScreenshotModal = false;
    screenshotTaken = false;
}

void ScreenCapture::update() {
    // No update logic required for now
}

void ScreenCapture::draw() {
    // No drawing logic required for now
}

void ScreenCapture::drawGui() {
    ImGui::Begin("Screen Capture");

    // Button to capture a screenshot
    if (ImGui::Button("Capture Screenshot")) {
        captureScreenshot();
        showScreenshotModal = true;
    }

    // Show screenshot preview modal if screenshot is taken
    if (showScreenshotModal && screenshotTaken) {
        ImGui::OpenPopup("Screenshot");

        if (ImGui::BeginPopupModal("Screenshot", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Screenshot Preview:");

            float aspectRatio = static_cast<float>(screenshotImg.getWidth()) / screenshotImg.getHeight();
            float newWidth = 400.0f;
            float newHeight = newWidth / aspectRatio;

            ImGui::Image((void*)(intptr_t)screenshotTexture, ImVec2(newWidth, newHeight));

            // Button to save the screenshot
            if (ImGui::Button("Save Screenshot")) {
                saveScreenshot();
                ImGui::CloseCurrentPopup();
                showScreenshotModal = false;
            }

            ImGui::SameLine();

            // Button to cancel the screenshot preview
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                showScreenshotModal = false;
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void ScreenCapture::exit() {
    deleteScreenshotTexture();
}

void ScreenCapture::captureScreenshot() {
    screenshotImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight()); // Capture the screen
    screenshotImg.update();
    createScreenshotTexture();
    screenshotTaken = true;
}

void ScreenCapture::saveScreenshot() {
    ofFileDialogResult result = ofSystemSaveDialog("screenshot.png", "Save your file");
    if (result.bSuccess) {
        screenshotImg.save(result.getPath());
    }
}

void ScreenCapture::createScreenshotTexture() {
    if (screenshotTexture != 0) {
        deleteScreenshotTexture(); // Delete previous texture if exists
    }

    // Generate a new texture from the screenshot
    glGenTextures(1, &screenshotTexture);
    glBindTexture(GL_TEXTURE_2D, screenshotTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, screenshotImg.getWidth(), screenshotImg.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, screenshotImg.getPixels().getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ScreenCapture::deleteScreenshotTexture() {
    if (screenshotTexture != 0) {
        glDeleteTextures(1, &screenshotTexture);
        screenshotTexture = 0;
    }
}
