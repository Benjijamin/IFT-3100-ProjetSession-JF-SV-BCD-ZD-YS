#include "AssetBrowser.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

void AssetBrowser::setup() {

}

void AssetBrowser::update() {

}

void AssetBrowser::draw() {

}

void AssetBrowser::drawGui() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("Asset Browser");

    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    float scrollableHeight = contentRegion.y - 100;

    ImGui::BeginChild("AssetList", ImVec2(0, scrollableHeight), true);

    auto filteredAssets = getFilteredAssets();
    for (const auto& asset : filteredAssets) {
        std::string displayText = showFullPaths ? asset : fs::path(asset).filename().string();
        if (ImGui::Selectable(displayText.c_str(), selectedAsset == asset)) {
            selectedAsset = asset;
            if (onAssetSelection) onAssetSelection();
        }
    }

    ImGui::EndChild();

    float windowPaddingY = ImGui::GetStyle().WindowPadding.y;
    ImGui::Dummy(ImVec2(0.0f, windowPaddingY));

    if (!selectedAsset.empty()) {
        if (ImGui::Button("Delete")) {
            removeAsset(selectedAsset);
            if (onAssetDeletion) onAssetDeletion();
            selectedAsset.clear();
        }
    }

    if (ImGui::Button("Load New Asset")) {
        ofFileDialogResult result = ofSystemLoadDialog("Select an asset");
        if (result.bSuccess) {
            addAsset(result.getPath());
        }
    }

    if (ImGui::Button("Load Assets from Folder")) {
        ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
        if (result.bSuccess) {
            loadAssetsFromFolder(result.getPath());
        }
    }

    const char* filterOptions[] = { "None", "Images", "Models" };
    ImGui::Combo("Filter", &filterIndex, filterOptions, IM_ARRAYSIZE(filterOptions));

    ImGui::Checkbox("Show Full Paths", &showFullPaths);

    ImGui::End();
}

void AssetBrowser::exit() {

}

void AssetBrowser::addAsset(const std::string& assetPath) {
    if (fs::exists(assetPath) && std::find(assets.begin(), assets.end(), assetPath) == assets.end()) {
        assets.push_back(assetPath);
    }
}

void AssetBrowser::removeAsset(const std::string& asset) {
    assets.erase(std::remove(assets.begin(), assets.end(), asset), assets.end());
}

void AssetBrowser::selectAsset(const std::string& assetPath) {
    auto it = std::find(assets.begin(), assets.end(), assetPath);
    if (it != assets.end()) {
        selectedAsset = assetPath;
    }
}

void AssetBrowser::loadAssetsFromFolder(const std::string& folderPath) {
    if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (fs::is_regular_file(entry.path())) {
                std::string assetPath = entry.path().string();
                if (isImageAsset(assetPath) || isModelAsset(assetPath)) {
                    addAsset(assetPath);
                }
            }
        }
    }
}

bool AssetBrowser::isImageAsset(const std::string& asset) const {
    std::string extension = fs::path(asset).extension().string();
    return extension == ".png" || extension == ".jpg" || extension == ".jpeg";
}

bool AssetBrowser::isModelAsset(const std::string& asset) const {
    std::string extension = fs::path(asset).extension().string();
    return extension == ".obj" || extension == ".dae" || extension == ".fbx";
}

std::vector<std::string> AssetBrowser::getFilteredAssets() const {
    std::vector<std::string> filteredAssets;
    std::copy_if(assets.begin(), assets.end(), std::back_inserter(filteredAssets), [&](const std::string& asset) {
        if (filterIndex == 1) {
            return isImageAsset(asset);
        }
        else if (filterIndex == 2) {
            return isModelAsset(asset);
        }
        return true;
        });
    return filteredAssets;
}

std::string AssetBrowser::getSelectedAssetPath() const {
    return selectedAsset;
}
