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

    assetBrowserHeight = std::max(assetBrowserHeight, 200.0f);
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - assetBrowserHeight));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, assetBrowserHeight));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Asset Browser", nullptr, windowFlags)) {
        drawNavigationBar();
        drawAssetList();

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                resizing = true;
            }
        }

        if (resizing && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            float mouseDelta = io.MouseDelta.y;
            assetBrowserHeight -= mouseDelta;
            assetBrowserHeight = std::clamp(assetBrowserHeight, 200.0f, windowSize.y / 2.0f);
        }
        else if (resizing && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            resizing = false;
        }
    }

    ImGui::End();
}

void AssetBrowser::drawNavigationBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

    ImGui::BeginChild("NavigationBar", ImVec2(0, 30), false, ImGuiWindowFlags_NoScrollbar);

    if (ImGui::Button("Load New Asset")) {
        ofFileDialogResult result = ofSystemLoadDialog("Select an asset");
        if (result.bSuccess) {
            addAsset(result.getPath());
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Load Assets from Folder")) {
        ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
        if (result.bSuccess) {
            loadAssetsFromFolder(result.getPath());
        }
    }

    ImGui::SameLine();

    if (!selectedAsset.empty()) {
        if (ImGui::Button("Save")) {
            std::string filename = fs::path(selectedAsset).filename().string();

            ofFileDialogResult result = ofSystemSaveDialog(filename, "Save your asset");
            if (result.bSuccess) {
                if (onAssetSave) onAssetSave(result.getPath());
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            removeAsset(selectedAsset);
            selectedAsset.clear();
        }
    }

    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 500.0f);

    const char* filterOptions[] = { "All", "Images", "Models" };
    ImGui::SetNextItemWidth(150);
    ImGui::Combo("", &filterIndex, filterOptions, IM_ARRAYSIZE(filterOptions));
    ImGui::SameLine();

    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##SearchBar", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
}

void AssetBrowser::drawAssetList() {
    ImU32 childBgColor = IM_COL32(30, 30, 30, 200);
    if (ImGui::GetStyle().Colors[ImGuiCol_WindowBg].x > 0.5f) {
        childBgColor = IM_COL32(255, 255, 255, 200);
    }

    ImGui::PushStyleColor(ImGuiCol_ChildBg, childBgColor);

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
    ImGui::PopStyleColor();
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
