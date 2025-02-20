#include "ImageEditor.h"

void ImageEditor::setup() {
    currentImage = new ofImage();
    panOffset.set(0, 0);
    zoomFactor = 1.0f;
    isDragging = false;
    drawRadius = 10;
    currentTool = Tool::PanZoom;

    colorPicker.setup();
    drawColor = colorPicker.getColor();
}

void ImageEditor::update() {

}

void ImageEditor::draw() {
    if (isImageAllocated()) {
        currentImage->draw(panOffset.x, panOffset.y, currentImage->getWidth() * zoomFactor, currentImage->getHeight() * zoomFactor);
    }

    if (currentTool == Tool::CopyRegion && isDragging) {
        drawCopyRegion();
    }
    else if (currentTool == Tool::PasteRegion && copiedRegion.isAllocated()) {
        drawPasteRegion();
    }
}

void ImageEditor::drawGui() {
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    float toolbarHeight = windowSize.y * 0.25f;
    ImGui::SetNextWindowSize(ImVec2(150, toolbarHeight));
    ImGui::SetNextWindowPos(ImVec2(0, (windowSize.y - toolbarHeight) / 2), ImGuiCond_FirstUseEver);
    ImGui::Begin("Toolbar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImVec2 buttonSize = ImVec2(-1, 40);

    if (ImGui::Button("Pan/Zoom", buttonSize)) {
        currentTool = Tool::PanZoom;
    }

    if (ImGui::Button("Copy Region", buttonSize)) {
        currentTool = Tool::CopyRegion;
    }

    if (ImGui::Button("Paste Region", buttonSize)) {
        currentTool = Tool::PasteRegion;
    }

    if (ImGui::Button("Brush", buttonSize)) {
        currentTool = Tool::Brush;
    }

    if (currentTool == Tool::Brush) {
        ImGui::SliderInt("Brush Size", &drawRadius, 1, 50);
    }

    ImGui::End();

    colorPicker.draw();
    drawColor = colorPicker.getColor();
}

void ImageEditor::exit() {
    delete currentImage;
    currentImage = nullptr;
}

void ImageEditor::mouseDragged(int x, int y, int button) {
    if (isNothingHovered() && isDragging) {
        switch (currentTool) {
        case Tool::PanZoom:
            panImage(x - dragStartPos.x, y - dragStartPos.y);
            dragStartPos = ofVec2f(x, y);
            break;
        case Tool::Brush:
            drawBrush(dragStartPos.x, dragStartPos.y, x, y);
            dragStartPos = ofVec2f(x, y);
            break;
        default:
            break;
        }
    }
}

void ImageEditor::mousePressed(int x, int y, int button) {
    dragStartPos.set(x, y);
    isDragging = true;

    if (currentTool == Tool::PasteRegion) {
        ofVec2f p = screenToPixelCoords(ofVec2f(x, y));
        pasteRegion(p.x, p.y);
    }
}

void ImageEditor::mouseReleased(int x, int y, int button) {
    dragEndPos.set(x, y);
    isDragging = false;

    if (currentTool == Tool::CopyRegion) {
        ofVec2f start = screenToPixelCoords(dragStartPos);
        ofVec2f end = screenToPixelCoords(dragEndPos);
        copyRegion(start.x, start.y, end.x, end.y);
    }
}

void ImageEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (currentTool == Tool::PanZoom && !ImGui::IsAnyWindowHovered()) {
        zoomImage(1.0f + scrollY * 0.1f, x, y);
    }
}

void ImageEditor::load(const std::string& path) {
    if (currentImage) {
        currentImage->load(path);
    }
    else {
        currentImage = new ofImage();
        currentImage->load(path);
    }

    adjustZoomAndPan();
}

void ImageEditor::save(const std::string& path) {
    if (isImageAllocated()) {
        currentImage->save(path);
    }
}

void ImageEditor::panImage(float dx, float dy) {
    panOffset.x += dx;
    panOffset.y += dy;
}

void ImageEditor::zoomImage(float scale, float mouseX, float mouseY) {
    ofVec2f prevMousePos = (ofVec2f(mouseX, mouseY) - panOffset) / zoomFactor;

    zoomFactor *= scale;
    if (zoomFactor < 0.1f) {
        zoomFactor = 0.1f;
    }
    else if (zoomFactor > 10.0f) {
        zoomFactor = 10.0f;
    }

    ofVec2f newMousePos = (ofVec2f(mouseX, mouseY) - panOffset) / zoomFactor;
    ofVec2f panAdjust = (newMousePos - prevMousePos) * zoomFactor;
    panOffset += panAdjust;
}

void ImageEditor::adjustZoomAndPan() {
    if (currentImage->isAllocated()) {
        int screenWidth = ofGetWidth();
        int screenHeight = ofGetHeight();
        int imageWidth = currentImage->getWidth();
        int imageHeight = currentImage->getHeight();

        float targetWidth = screenWidth * 0.8f;
        zoomFactor = targetWidth / imageWidth;

        panOffset.set((screenWidth - imageWidth * zoomFactor) / 2, (screenHeight - imageHeight * zoomFactor) / 2);
    }
}

void ImageEditor::drawBrush(int startX, int startY, int endX, int endY) {
    ofVec2f start = screenToPixelCoords(ofVec2f(startX, startY));
    ofVec2f end = screenToPixelCoords(ofVec2f(endX, endY));

    int numSteps = std::max(abs(end.x - start.x), abs(end.y - start.y));
    for (int step = 0; step <= numSteps; ++step) {
        float t = static_cast<float>(step) / static_cast<float>(numSteps);
        int x = static_cast<int>(ofLerp(start.x, end.x, t));
        int y = static_cast<int>(ofLerp(start.y, end.y, t));

        for (int i = -drawRadius; i <= drawRadius; ++i) {
            for (int j = -drawRadius; j <= drawRadius; ++j) {
                if (i * i + j * j <= drawRadius * drawRadius) {
                    int drawX = x + i;
                    int drawY = y + j;
                    if (isWithinBounds(drawX, drawY)) {
                        currentImage->setColor(drawX, drawY, drawColor);
                    }
                }
            }
        }
    }
    currentImage->update();
}

void ImageEditor::copyRegion(int startX, int startY, int endX, int endY) {
    if (endX < startX) std::swap(startX, endX);
    if (endY < startY) std::swap(startY, endY);

    int width = endX - startX;
    int height = endY - startY;

    copiedRegion.allocate(width, height, OF_IMAGE_COLOR);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            copiedRegion.setColor(x, y, currentImage->getColor(startX + x, startY + y));
        }
    }
    copiedRegion.update();
}

void ImageEditor::pasteRegion(int x, int y) {
    if (!copiedRegion.isAllocated()) return;

    for (int i = 0; i < copiedRegion.getWidth(); ++i) {
        for (int j = 0; j < copiedRegion.getHeight(); ++j) {
            int drawX = x + i;
            int drawY = y + j;
            if (isWithinBounds(drawX, drawY)) {
                currentImage->setColor(drawX, drawY, copiedRegion.getColor(i, j));
            }
        }
    }
    currentImage->update();
}

void ImageEditor::drawCopyRegion() {
    ofVec2f corner1 = dragStartPos;
    ofVec2f corner2 = ofVec2f(ofGetMouseX(), ofGetMouseY());

    int x = std::min(corner1.x, corner2.x);
    int y = std::min(corner1.y, corner2.y);
    int width = std::abs(corner2.x - corner1.x);
    int height = std::abs(corner2.y - corner1.y);

    ofPushStyle();
    ofSetColor(0, 122, 204, 128);
    ofNoFill();
    ofDrawRectangle(x, y, width, height);
    ofPopStyle();
}

void ImageEditor::drawPasteRegion() {
    int x = ofGetMouseX();
    int y = ofGetMouseY();
    int width = copiedRegion.getWidth() * zoomFactor;
    int height = copiedRegion.getHeight() * zoomFactor;

    ofPushStyle();
    ofSetColor(128, 128, 128, 128);
    ofFill();
    ofDrawRectangle(x, y, width, height);
    ofPopStyle();
}

bool ImageEditor::isNothingHovered() {
    return !ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered();
}

bool ImageEditor::isImageAllocated() const {
    return currentImage && currentImage->isAllocated();
}

bool ImageEditor::isWithinBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < currentImage->getWidth() && y < currentImage->getHeight();
}

ofVec2f ImageEditor::screenToPixelCoords(const ofVec2f& screenCoords) const {
    return (screenCoords - panOffset) / zoomFactor;
}

ofVec2f ImageEditor::pixelToScreenCoords(const ofVec2f& pixelCoords) const {
    return pixelCoords * zoomFactor + panOffset;
}
