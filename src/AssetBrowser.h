#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ImageEditor.h"

class AssetBrowser {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    std::string getSelectedAssetPath() const;
    std::string getLastAssetPath() const;

    static bool isImageAsset(const std::string& asset);
    static bool isModelAsset(const std::string& asset);

    std::function<void()> onAssetAddition;
    std::function<void()> onAssetRemoval;
    std::function<void()> onAssetSelection;
    std::function<void(const std::string&)> onAssetSave;

private:
    void drawNavigationBar();
    void drawAssetList();

    void addAsset(const std::string& assetPath);
    void removeAsset(const std::string& asset);
    void selectAsset(const std::string& assetPath);
    void loadAssetsFromFolder(const std::string& folderPath);

    std::vector<std::string> getFilteredAssets() const;

    std::vector<std::string> assets;
    std::string selectedAsset;

    char searchBuffer[128];
    float assetBrowserHeight;
    bool resizing;
    bool showFullPaths;
    int filterIndex;

    ImageEditor imageEditor;
};
