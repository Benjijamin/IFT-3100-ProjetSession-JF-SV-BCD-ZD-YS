#include "AssetBrowser.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

void AssetBrowser::setup() {
    assetBrowserHeight = 210.0f;
    resizing = false;
    showFullPaths = false;
    filterIndex = 0;
}

void AssetBrowser::update() {

}

void AssetBrowser::draw() {

}

void AssetBrowser::drawGui() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize = io.DisplaySize;

    float controlsHeight = calculateControlsHeight();

    assetBrowserHeight = std::max(assetBrowserHeight, controlsHeight);

    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - assetBrowserHeight));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, assetBrowserHeight));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("Asset Browser", nullptr, windowFlags)) {
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                resizing = true;
            }
        }

        if (resizing && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            float mouseDelta = io.MouseDelta.y;
            assetBrowserHeight -= mouseDelta;
            assetBrowserHeight = std::clamp(assetBrowserHeight, controlsHeight, windowSize.y / 2.0f);
        }
        else if (resizing && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            resizing = false;
        }

        ImU32 childBgColor = IM_COL32(30, 30, 30, 200);
        if (ImGui::GetStyle().Colors[ImGuiCol_WindowBg].x > 0.5f) {
            childBgColor = IM_COL32(255, 255, 255, 200);
        }

        ImGui::PushStyleColor(ImGuiCol_ChildBg, childBgColor);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, windowSize.x * 0.75f);

        drawSearchBar();
        drawAssetList();

        ImGui::NextColumn();

        ImGui::PopStyleColor();

        drawControls();

        ImGui::Columns(1);
        ImGui::PopStyleVar(2);
    }

    ImGui::End();
}

void AssetBrowser::drawSearchBar() {
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    ImGui::Dummy(ImVec2(0.0f, 4.0f));
}

void AssetBrowser::drawAssetList() {
    ImGui::BeginChild("AssetList", ImVec2(0, 0), false);

    auto filteredAssets = getFilteredAssets();

    for (size_t i = 0; i < filteredAssets.size(); ++i) {
        const auto& asset = filteredAssets[i];
        std::string displayText = showFullPaths ? asset : fs::path(asset).stem().string();

        if (strstr(displayText.c_str(), searchBuffer) != nullptr) {
            ImVec2 selectableSize = ImVec2(0, ImGui::GetFrameHeightWithSpacing());

            std::string selectableID = "##hidden" + std::to_string(i);
            if (ImGui::Selectable(selectableID.c_str(), selectedAsset == asset, 0, selectableSize)) {
                selectAsset(asset);
            }

            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("ASSET", asset.c_str(), asset.size() + 1);
                ImGui::Text(asset.c_str());
                ImGui::EndDragDropSource();
            }

            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (selectableSize.y - ImGui::GetTextLineHeight()) / 2);
            ImGui::Text(displayText.c_str());

            if (!showFullPaths) {
                ImGui::SameLine();
                ImGui::TextDisabled("[%s]", fs::path(asset).extension().string().c_str());
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (selectableSize.y - ImGui::GetTextLineHeight()) / 2);
        }
    }

    ImGui::EndChild();
}

void AssetBrowser::drawControls() {
    ImGui::BeginChild("Controls", ImVec2(0, 0), false);

    ImVec2 buttonSize = ImVec2(-1, 0);

    if (!selectedAsset.empty()) {
        if (ImGui::Button("Save", buttonSize)) {
            imageExporter.image_export("myImage", "png", imageEditor);
        }
    }

    if (!selectedAsset.empty()) {
        if (ImGui::Button("Delete", buttonSize)) {
            removeAsset(selectedAsset);
            selectedAsset.clear();
        }
    }


    if (ImGui::Button("Load New Asset", buttonSize)) {
        ofFileDialogResult result = ofSystemLoadDialog("Select an asset");
        if (result.bSuccess) {
            addAsset(result.getPath());
        }
    }

    if (ImGui::Button("Load Assets from Folder", buttonSize)) {
        ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
        if (result.bSuccess) {
            loadAssetsFromFolder(result.getPath());
        }
    }

    const char* filterOptions[] = { "None", "Images", "Models" };
    ImGui::Combo("Filter", &filterIndex, filterOptions, IM_ARRAYSIZE(filterOptions));

    ImGui::Checkbox("Show Full Paths", &showFullPaths);

    ImGui::EndChild();
}

float AssetBrowser::calculateControlsHeight() {
    ImVec2 framePadding = ImVec2(10, 10);
    float elementHeight = ImGui::GetFrameHeight() + framePadding.y * 2;

    int elementCount = !selectedAsset.empty() ? 5 : 4;
    int offset = !selectedAsset.empty() ? 21 : 23;

    return elementCount * elementHeight + offset;
}

void AssetBrowser::exit() {

}

std::string AssetBrowser::getSelectedAssetPath() const {
    return selectedAsset;
}

std::string AssetBrowser::getLastAssetPath() const {
    return assets.back();
}

void AssetBrowser::addAsset(const std::string& assetPath) {
    if (!fs::exists(assetPath)) {
        return;
    }

    if (std::find(assets.begin(), assets.end(), assetPath) == assets.end()) {
        assets.push_back(assetPath);
    }

    if (onAssetAddition) onAssetAddition();
}

void AssetBrowser::removeAsset(const std::string& asset) {
    auto it = std::remove(assets.begin(), assets.end(), asset);
    if (it != assets.end()) {
        assets.erase(it, assets.end());
    }

    if (onAssetRemoval) onAssetRemoval();
}

void AssetBrowser::selectAsset(const std::string& assetPath) {
    if (std::find(assets.begin(), assets.end(), assetPath) != assets.end()) {
        selectedAsset = assetPath;
    }

    if (onAssetSelection) onAssetSelection();
}

void AssetBrowser::loadAssetsFromFolder(const std::string& folderPath) {
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        return;
    }

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            std::string assetPath = entry.path().string();
            if (isImageAsset(assetPath) || isModelAsset(assetPath)) {
                addAsset(assetPath);
            }
        }
    }
}

bool AssetBrowser::isImageAsset(const std::string& asset){
    static const std::set<std::string> imageExtensions = { ".png", ".jpg", ".jpeg" };
    return imageExtensions.find(fs::path(asset).extension().string()) != imageExtensions.end();
}

bool AssetBrowser::isModelAsset(const std::string& asset){
    static const std::set<std::string> modelExtensions = { ".obj", ".dae", ".fbx" };
    return modelExtensions.find(fs::path(asset).extension().string()) != modelExtensions.end();
}

std::vector<std::string> AssetBrowser::getFilteredAssets() const {
    std::vector<std::string> filteredAssets;
    std::copy_if(assets.begin(), assets.end(), std::back_inserter(filteredAssets), [&](const std::string& asset) {
        switch (filterIndex) {
        case 1:
            return isImageAsset(asset);
        case 2:
            return isModelAsset(asset);
        default:
            return true;
        }
        });
    return filteredAssets;
}
