#pragma once
#include "ofMain.h"
#include "FreeFlightCamera.h"
#include "ofxImGui.h"

class CameraManager {
public:
    void setup();
    void update();
    void draw();
    void drawGui();
    void exit();

    void addOrbitCamera(const std::string& name, const glm::vec3& position, const glm::vec3& target = glm::vec3(0.0f));
    void addFreeFlightCamera(const std::string& name, const glm::vec3& position);

    void adjustCameraToFitAABB(const ofBoxPrimitive& aabb);

    void setSelectedCamera(int index);
    ofCamera* getSelectedCamera();

    void setMouseInput(ofCamera* camera, bool enable);
    void enableSelectedMouseInput();
    void disableAllMouseInput();

    ofCamera* getFrustumCamera();
    bool isFrustumCullingEnabled() const;

private:
    void drawFreeFlightParameters();
    void drawOrbitParameters();
    void drawPerspectiveParameters();
    void drawOrthographicParameters();

    void switchCameraType(bool toFreeFlight);
    std::string generateUniqueName(const std::string& baseName);

    struct CameraData {
        std::string name;
        ofCamera* camera;
        bool isFreeFlight;
    };

    std::vector<CameraData> cameras;
    int selectedCameraIndex = -1;
    int frustumCameraIndex = -1;
    bool frustumCullingEnabled = false;
};
