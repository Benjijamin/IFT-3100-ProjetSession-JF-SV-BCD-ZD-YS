#include "AssetBrowser.h"
#include <filesystem>

namespace fs = std::filesystem;

void AssetBrowser::drawGui() {
    ImGui::Begin("Asset Browser");

    for (size_t i = 0; i < assets.size(); ++i) {
        const auto& asset = assets[i];
        if (ImGui::Selectable(asset.c_str(), selectedAsset == asset)) {
            selectedAsset = asset;
            if (onAssetSelection) onAssetSelection();
        }
    }

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

    ImGui::End();
}

void AssetBrowser::addAsset(const std::string& assetPath) {
    if (fs::exists(assetPath)) {
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

bool AssetBrowser::isImageAsset(const std::string& asset) const {
    std::string extension = fs::path(asset).extension().string();
    return extension == ".png" || extension == ".jpg" || extension == ".jpeg";
}

bool AssetBrowser::isModelAsset(const std::string& asset) const {
    std::string extension = fs::path(asset).extension().string();
    return extension == ".obj" || extension == ".dae" || extension == ".fbx";
}

std::string AssetBrowser::getSelectedAssetPath() const {
    return selectedAsset;
}
