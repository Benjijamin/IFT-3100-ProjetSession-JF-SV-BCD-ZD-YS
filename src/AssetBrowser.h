#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class AssetBrowser {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void addAsset(const std::string& assetPath);
    void removeAsset(const std::string& asset);
    void selectAsset(const std::string& assetPath);

    bool isImageAsset(const std::string& asset) const;
    bool isModelAsset(const std::string& asset) const;

    std::string getSelectedAssetPath() const;

    std::function<void()> onAssetSelection;
    std::function<void()> onAssetDeletion;

private:
    std::vector<std::string> assets;
    std::string selectedAsset; 
};
