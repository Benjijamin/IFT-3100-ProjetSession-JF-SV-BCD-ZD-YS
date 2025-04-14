#include "ImageEditor.h"

void ImageEditor::setup() {
    currentImage = new ofImage();
    drawRadius = 10;
    currentTool = Tool::PanZoom;

    colorPicker.setup();
    drawColor = colorPicker.getColor();

    filterShader.load("image_filter_330_vs.glsl", "image_filter_330_fs.glsl");

    tintColor = ofColor(0);
    saturationValue = 1.0f;
    brightnessValue = 0.0f;
    contrastValue = 1.0f;
    blurValue = 0.0f;

    ofDisableArbTex();
}

void ImageEditor::update() {
    imageDimensions = ofVec2f(currentImage->getWidth(), currentImage->getHeight()) * viewer.getZoomFactor();

    if (currentTool == Tool::Tint) 
    {
        tintColor = colorPicker.getColor();
    }
}

void ImageEditor::draw() {
    if (isImageAllocated()) {
        ofVec2f panOffset = viewer.getPanOffset();
        ofVec2f imageDimensions = ofVec2f(currentImage->getWidth(), currentImage->getHeight()) * viewer.getZoomFactor();

        applyFilter(panOffset);
    }

    if (currentTool == Tool::CopyRegion && viewer.isDraggingMouse() && !isGuiHovered()) {
        drawCopyRegion();
    }
    else if (currentTool == Tool::PasteRegion && copiedRegion.isAllocated() && !isGuiHovered()) {
        drawPasteRegion();
    }

}

void ImageEditor::drawGui() {
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    float toolbarWidth = 150.0f;
    float padding = 15.0f;

    ImGui::SetNextWindowSizeConstraints(ImVec2(toolbarWidth, 0), ImVec2(toolbarWidth, windowSize.y));
    ImGui::Begin("Toolbar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImVec2 buttonSize = ImVec2(-1, 40);

    if (ImGui::Button("Pan/Zoom", buttonSize))
    {
        switchTool(Tool::PanZoom);
    }

    if (ImGui::Button("Copy Region", buttonSize))
    {
        switchTool(Tool::CopyRegion);
    }

    if (ImGui::Button("Paste Region", buttonSize))
    {
        switchTool(Tool::PasteRegion);
    }

    if (ImGui::Button("Brush", buttonSize)) 
    {
        switchTool(Tool::Brush);
    }

    if (currentTool == Tool::Brush) 
    {
        ImGui::Text("Brush Size");
        ImGui::SetNextItemWidth(toolbarWidth - padding);
        ImGui::SliderInt("##Brush Size", &drawRadius, 1, 50);
    }

    if (ImGui::Button("Tint", buttonSize))
    {
        switchTool(Tool::Tint);
    }

    if (ImGui::Button("Vibrance", buttonSize)) 
    {
        switchTool(Tool::Vibrance);
    }

    if (currentTool == Tool::Vibrance) 
    {
        ImGui::Text("Saturation Value");
        ImGui::SetNextItemWidth(toolbarWidth - padding);
        ImGui::SliderFloat("##Saturation Value", &saturationValue, 0.0f, 2.0f);
        ImGui::Text("Brightness Value");
        ImGui::SetNextItemWidth(toolbarWidth - padding);
        ImGui::SliderFloat("##Brightness Value", &brightnessValue, -1.0f, 1.0f);
        ImGui::Text("Contrast Value");
        ImGui::SetNextItemWidth(toolbarWidth - padding);
        ImGui::SliderFloat("##Contrast Value", &contrastValue, 0.0f, 2.0f);
    }

    if (ImGui::Button("Blur", buttonSize)) 
    {
        switchTool(Tool::Blur);
    }

    if (currentTool == Tool::Blur) 
    {
        ImGui::Text("Blur Value");
        ImGui::SetNextItemWidth(toolbarWidth - padding);
        ImGui::SliderFloat("##Blur Value", &blurValue, 0.0f, 10.0f);
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
    if (viewer.isDraggingMouse() && !isGuiHovered()) {
        switch (currentTool) {
        case Tool::PanZoom:
            viewer.mouseDragged(x, y, button);
            break;
        case Tool::Brush:
            drawBrush(previousMousePos.x, previousMousePos.y, x, y);
            previousMousePos = ofVec2f(x, y);
            break;
        default:
            break;
        }
    }
}

void ImageEditor::mousePressed(int x, int y, int button) {
    viewer.mousePressed(x, y, button);

    previousMousePos = ofVec2f(x, y);

    if (currentTool == Tool::PasteRegion && !isGuiHovered()) {
        ofVec2f p = viewer.screenToPixelCoords(ofVec2f(x, y));
        pasteRegion(p.x, p.y);
    }
}

void ImageEditor::mouseReleased(int x, int y, int button) {
    viewer.mouseReleased(x, y, button);

    if (currentTool == Tool::CopyRegion && !isGuiHovered()) {
        ofVec2f start = viewer.screenToPixelCoords(viewer.getDragStartPos());
        ofVec2f end = viewer.screenToPixelCoords(viewer.getDragEndPos());
        copyRegion(start.x, start.y, end.x, end.y);
    }
}

void ImageEditor::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (currentTool == Tool::PanZoom) {
        viewer.mouseScrolled(x, y, scrollX, scrollY);
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

    viewer.adjustZoomAndPan(currentImage->getWidth(), currentImage->getHeight());
}

void ImageEditor::save(const std::string& path) {
    if (isImageAllocated()) {
        bakeFilter();
        currentImage->save(path);
    }
}

void ImageEditor::switchTool(Tool tool) 
{
    currentTool = tool;
}

void ImageEditor::drawBrush(int startX, int startY, int endX, int endY) {
    ofVec2f start = viewer.screenToPixelCoords(ofVec2f(startX, startY));
    ofVec2f end = viewer.screenToPixelCoords(ofVec2f(endX, endY));

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
    originalImage = new ofImage();
    *originalImage = *currentImage;
}

void ImageEditor::setFilterValues() 
{
    filterShader.setUniformTexture("image", currentImage->getTexture(), 1);

    filterShader.setUniform2f("resolution", imageDimensions);

    filterShader.setUniform4f("tint", tintColor.r, tintColor.g, tintColor.b, 1.0f);

    filterShader.setUniform1f("saturationValue", saturationValue);
    filterShader.setUniform1f("brightnessValue", brightnessValue);
    filterShader.setUniform1f("contrastValue", contrastValue);

    filterShader.setUniform1f("blurValue", blurValue);
}

void ImageEditor::applyFilter(ofVec2f panOffset) 
{
    if (originalImage) {
        *currentImage = *originalImage;
        currentImage->update();
    }

    filterShader.begin();

    setFilterValues();

    currentImage->draw(panOffset.x, panOffset.y, imageDimensions.x, imageDimensions.y);

    filterShader.end();
}

void ImageEditor::bakeFilter() {
    if (!isImageAllocated()) return;

    if (!originalImage) {
        originalImage = new ofImage();
        *originalImage = *currentImage;
    }

    ofFbo fbo;
    fbo.allocate(currentImage->getWidth(), currentImage->getHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);

    filterShader.begin();

    setFilterValues();

    fbo.end();

    ofPixels pixels;
    fbo.readToPixels(pixels);
    currentImage->setFromPixels(pixels);
    currentImage->update();

    filterShader.end();
}

void ImageEditor::copyRegion(int startX, int startY, int endX, int endY) {
    if (endX < startX) std::swap(startX, endX);
    if (endY < startY) std::swap(startY, endY);

    startX = std::clamp(startX, 0, static_cast<int>(currentImage->getWidth() - 1));
    startY = std::clamp(startY, 0, static_cast<int>(currentImage->getHeight() - 1));
    endX = std::clamp(endX, 0, static_cast<int>(currentImage->getWidth() - 1));
    endY = std::clamp(endY, 0, static_cast<int>(currentImage->getHeight() - 1));

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
    originalImage = new ofImage();
    *originalImage = *currentImage;


}

void ImageEditor::drawCopyRegion() {
    ofVec2f corner1 = viewer.getDragStartPos();
    ofVec2f corner2 = ofVec2f(ofGetMouseX(), ofGetMouseY());

    int x = std::min(corner1.x, corner2.x);
    int y = std::min(corner1.y, corner2.y);
    int width = std::abs(corner2.x - corner1.x);
    int height = std::abs(corner2.y - corner1.y);

    ofPushStyle();
    ofSetColor(0, 122, 204, 255);
    ofSetLineWidth(5);
    ofNoFill();
    ofDrawRectangle(x, y, width, height);
    ofPopStyle();
}

void ImageEditor::drawPasteRegion() {
    int x = ofGetMouseX();
    int y = ofGetMouseY();
    int width = copiedRegion.getWidth() * viewer.getZoomFactor();
    int height = copiedRegion.getHeight() * viewer.getZoomFactor();

    ofPushStyle();
    ofSetColor(128, 128, 128, 128);
    ofFill();
    ofDrawRectangle(x, y, width, height);
    ofPopStyle();
}

bool ImageEditor::isGuiHovered() {
    return ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered();
}

bool ImageEditor::isImageAllocated() const {
    return currentImage && currentImage->isAllocated();
}

bool ImageEditor::isWithinBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < currentImage->getWidth() && y < currentImage->getHeight();
}