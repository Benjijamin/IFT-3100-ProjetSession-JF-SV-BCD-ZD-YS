#include "ScreenCapture.h"

void ScreenCapture::setup() {
    screenshotTexture = 0;
    showScreenshotModalFlag = false;
    screenshotTaken = false;
}

void ScreenCapture::update() {

}

void ScreenCapture::draw() {

}

void ScreenCapture::drawGui() {
    if (showScreenshotModalFlag && screenshotTaken) {
        ImGui::OpenPopup("Screenshot");

        if (ImGui::BeginPopupModal("Screenshot", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Screenshot Preview:");

            float aspectRatio = static_cast<float>(screenshotImg.getWidth()) / screenshotImg.getHeight();
            float newWidth = 800.0f;
            float newHeight = newWidth / aspectRatio;

            ImGui::Image((void*)(intptr_t)screenshotTexture, ImVec2(newWidth, newHeight));

            if (ImGui::Button("Save Screenshot")) {
                saveScreenshot();
                ImGui::CloseCurrentPopup();
                hideModal();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                hideModal();
            }

            ImGui::EndPopup();
        }
    }
}

void ScreenCapture::exit() {
    deleteScreenshotTexture();
}

void ScreenCapture::captureScreenshot() {
    screenshotImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    screenshotImg.update();
    createScreenshotTexture();
    screenshotTaken = true;
    showModal();
}

void ScreenCapture::saveScreenshot() {
    ofFileDialogResult result = ofSystemSaveDialog("screenshot.png", "Save your file");
    if (result.bSuccess) {
        screenshotImg.save(result.getPath());
    }
}

void ScreenCapture::createScreenshotTexture() {
    if (screenshotTexture != 0) {
        deleteScreenshotTexture();
    }

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

void ScreenCapture::showModal() {
    showScreenshotModalFlag = true;
}

void ScreenCapture::hideModal() {
    showScreenshotModalFlag = false;
}
