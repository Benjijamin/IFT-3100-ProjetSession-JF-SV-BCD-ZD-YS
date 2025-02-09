#include "ImageEditor.h"

void ImageEditor::setup() {
    currentImage = new ofImage();
    panOffset.set(0, 0);
    zoomFactor = 1.0f;
    isDrawing = false;
    drawRadius = 10;
    currentTool = Tool::PanZoom;

    colorPicker.setup();
    drawColor = ofColor(colorPicker.selectedColor[0] * 255, colorPicker.selectedColor[1] * 255,
        colorPicker.selectedColor[2] * 255);
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
    ImGui::Begin("Toolbar", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Pan/Zoom")) {
        currentTool = Tool::PanZoom;
    }

    ImGui::SameLine();

    if (ImGui::Button("Circle")) {
        currentTool = Tool::Circle;
    }

    ImGui::SameLine();

    if (ImGui::Button("Square")) {
        currentTool = Tool::Square;
    }

    ImGui::SameLine();

    if (ImGui::Button("Copy Region")) {
        currentTool = Tool::CopyRegion;
    }

    ImGui::SameLine();

    if (ImGui::Button("Paste Region")) {
        currentTool = Tool::PasteRegion;
    }

    if (currentTool == Tool::Circle || currentTool == Tool::Square) {

        ImVec4 color(colorPicker.selectedColor[0], colorPicker.selectedColor[1], 
            colorPicker.selectedColor[2], colorPicker.selectedColor[3]);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if (ImGui::Button("", ImVec2(16, 16)))
        {
            colorPicker.showColorPicker = true;
        }
        ImGui::PopStyleColor(1);

        ImGui::SameLine();

        ImGui::SliderInt("Draw Radius", &drawRadius, 1, 50);
        
    }

    if (colorPicker.showColorPicker) 
    {
        colorPicker.draw();
        drawColor = ofColor(colorPicker.selectedColor[0] * 255, colorPicker.selectedColor[1] * 255,
            colorPicker.selectedColor[2] * 255);
    }

    ImGui::End();
}

void ImageEditor::exit() {
    delete currentImage;
    currentImage = nullptr;
}

void ImageEditor::mouseDragged(int x, int y, int button) {
    ofVec2f p = screenToPixelCoords(ofVec2f(x, y));

    switch (currentTool) {
    case Tool::PanZoom:
        panImage(x - dragStartPos.x, y - dragStartPos.y);
        dragStartPos = ofVec2f(x, y);
        break;
    case Tool::Circle:
        drawCircle(p.x, p.y);
        break;
    case Tool::Square:
        drawSquare(p.x, p.y);
        break;
    default:
        break;
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
    if (currentTool == Tool::PanZoom) {
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

void ImageEditor::drawCircle(int x, int y) {
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
    currentImage->update();
}

void ImageEditor::drawSquare(int x, int y) {
    for (int i = -drawRadius; i <= drawRadius; ++i) {
        for (int j = -drawRadius; j <= drawRadius; ++j) {
            int drawX = x + i;
            int drawY = y + j;
            if (isWithinBounds(drawX, drawY)) {
                currentImage->setColor(drawX, drawY, drawColor);
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
