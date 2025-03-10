#include "CameraManager.h"

void CameraManager::setup() {

}

void CameraManager::update() {

}

void CameraManager::draw() {

}

void CameraManager::drawGui() {
    ImGui::Begin("Camera Controls");

    if (ImGui::CollapsingHeader("Camera List")) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
            ImGui::OpenPopup("CameraContextMenu");
        }

        for (int i = 0; i < cameras.size(); i++) {
            if (ImGui::Selectable(cameras[i].name.c_str(), i == selectedCameraIndex)) {
                setSelectedCamera(i);
            }
        }

        if (ImGui::BeginPopup("CameraContextMenu")) {
            static char cameraName[128] = "";
            ImGui::InputText("Camera Name", cameraName, IM_ARRAYSIZE(cameraName));

            if (ImGui::Button("Add Orbit Camera") && strlen(cameraName) > 0) {
                std::string uniqueName = generateUniqueName(cameraName);
                addOrbitCamera(uniqueName, glm::vec3(0, 0, 500), glm::vec3(0, 0, 0));
                cameraName[0] = '\0';
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Add Free Flight Camera") && strlen(cameraName) > 0) {
                std::string uniqueName = generateUniqueName(cameraName);
                addFreeFlightCamera(uniqueName, glm::vec3(0, 500, 500));
                cameraName[0] = '\0';
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Delete Selected Camera") && cameras.size() > 1) {
                delete cameras[selectedCameraIndex].camera;
                cameras.erase(cameras.begin() + selectedCameraIndex);

                if (selectedCameraIndex > 0) {
                    setSelectedCamera(selectedCameraIndex - 1);
                }
                else {
                    setSelectedCamera(0);
                }
            }

            ImGui::EndPopup();
        }
    }

    if (ImGui::CollapsingHeader("Projection Parameters")) {
        ofCamera* activeCam = getSelectedCamera();
        static bool isPerspective = !activeCam->getOrtho();

        if (ImGui::RadioButton("Perspective", isPerspective)) {
            isPerspective = true;
            activeCam->disableOrtho();
        }

        ImGui::SameLine();

        if (ImGui::RadioButton("Orthographic", !isPerspective)) {
            isPerspective = false;
            activeCam->enableOrtho();
        }

        if (isPerspective) {
            drawPerspectiveParameters();
        }
        else {
            drawOrthographicParameters();
        }
    }

    if (ImGui::CollapsingHeader("Camera Type Parameters")) {
        ofCamera* activeCam = getSelectedCamera();

        bool isFreeFlight = cameras[selectedCameraIndex].isFreeFlight;
        ImGui::Text("Type: %s", isFreeFlight ? "Free Flight Camera" : "Orbit Camera");

        if (isFreeFlight) {
            drawFreeFlightParameters();
        }
        else {
            drawOrbitParameters();
        }
    }

    if (ImGui::CollapsingHeader("Frustum Culling")) {
        if (ImGui::Checkbox("Enable Frustum Culling", &frustumCullingEnabled)) {

        }

        if (frustumCullingEnabled) {
            if (frustumCameraIndex < 0 || frustumCameraIndex >= cameras.size()) {
                frustumCameraIndex = 0;
            }

            if (ImGui::BeginCombo("Select Camera", cameras[frustumCameraIndex].name.c_str())) {
                for (int i = 0; i < cameras.size(); i++) {
                    bool isSelected = (i == frustumCameraIndex);

                    if (ImGui::Selectable(cameras[i].name.c_str(), isSelected)) {
                        frustumCameraIndex = i;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
    }

    ImGui::End();
}

void CameraManager::drawFreeFlightParameters() {
    if (auto* freeFlightCam = dynamic_cast<FreeFlightCamera*>(getSelectedCamera())) {
        float moveSpeed = freeFlightCam->getMoveSpeed();
        if (ImGui::SliderFloat("Move Speed", &moveSpeed, 1.0f, 50.0f)) {
            freeFlightCam->setMoveSpeed(moveSpeed);
        }
    
        float sensitivity = freeFlightCam->getSensitivity();
        if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.01f, 1.0f)) {
            freeFlightCam->setSensitivity(sensitivity);
        }
    }
}

void CameraManager::drawOrbitParameters() {
    if (auto* orbitCam = dynamic_cast<ofEasyCam*>(getSelectedCamera())) {
        float distance = orbitCam->getDistance();
        if (ImGui::SliderFloat("Camera Distance", &distance, 50.0f, 5000.0f)) {
            orbitCam->setDistance(distance);
        }
    }
}

void CameraManager::drawOrthographicParameters() {
    ofCamera* activeCam = getSelectedCamera();
    if (activeCam && activeCam->getOrtho()) {
        float nearClip = activeCam->getNearClip();
        if (ImGui::SliderFloat("Near Clip", &nearClip, 0.01f, 10.0f)) {
            activeCam->setNearClip(nearClip);
        }
    
        float farClip = activeCam->getFarClip();
        if (ImGui::SliderFloat("Far Clip", &farClip, 100.0f, 10000.0f)) {
            activeCam->setFarClip(farClip);
        }
    }
}

void CameraManager::drawPerspectiveParameters() {
    ofCamera* activeCam = getSelectedCamera();
    if (activeCam && !activeCam->getOrtho()) {
        float fov = activeCam->getFov();
        if (ImGui::SliderFloat("Field of View", &fov, 30.0f, 120.0f)) {
            activeCam->setFov(fov);
        }
    
        float aspect = activeCam->getAspectRatio();
        if (ImGui::SliderFloat("Aspect Ratio", &aspect, 1.0f, 2.0f)) {
            activeCam->setAspectRatio(aspect);
        }
    
        float nearClip = activeCam->getNearClip();
        if (ImGui::SliderFloat("Near Clip", &nearClip, 0.01f, 10.0f)) {
            activeCam->setNearClip(nearClip);
        }
    
        float farClip = activeCam->getFarClip();
        if (ImGui::SliderFloat("Far Clip", &farClip, 100.0f, 10000.0f)) {
            activeCam->setFarClip(farClip);
        }
    }
}

void CameraManager::exit() {

}

void CameraManager::addOrbitCamera(const std::string& name, const glm::vec3& position, const glm::vec3& target) {
    auto* orbitCam = new ofEasyCam();
    orbitCam->setPosition(position);
    orbitCam->lookAt(target);
    CameraData data = { name, orbitCam, false };
    cameras.push_back(data);

    if (cameras.size() == 1) {
        setSelectedCamera(0);
    }
}

void CameraManager::addFreeFlightCamera(const std::string& name, const glm::vec3& position) {
    auto* freeFlightCam = new FreeFlightCamera();
    freeFlightCam->setPosition(position);
    CameraData data = { name, freeFlightCam, true };
    cameras.push_back(data);

    if (cameras.size() == 1) {
        setSelectedCamera(0);
    }
}

void CameraManager::switchCameraType(bool toFreeFlight) {
    if (selectedCameraIndex >= 0 && selectedCameraIndex < cameras.size()) {
        auto& selectedCameraData = cameras[selectedCameraIndex];

        if (selectedCameraData.isFreeFlight == toFreeFlight) {
            return;
        }

        std::string currentName = selectedCameraData.name;
        glm::vec3 position = selectedCameraData.camera->getGlobalPosition();

        delete selectedCameraData.camera;

        if (toFreeFlight) {
            selectedCameraData.camera = new FreeFlightCamera();
            selectedCameraData.isFreeFlight = true;
        }
        else {
            selectedCameraData.camera = new ofEasyCam();
            selectedCameraData.isFreeFlight = false;
        }

        selectedCameraData.camera->setGlobalPosition(position);
        selectedCameraData.name = currentName;
    }
}

void CameraManager::adjustCameraToFitAABB(const ofBoxPrimitive& aabb) {
    ofCamera* camera = getSelectedCamera();
    if (!camera) return;

    glm::vec3 aabbCenter = aabb.getPosition();
    glm::vec3 aabbSize = aabb.getSize();
    float maxDimension = glm::length(aabbSize);

    float paddingFactor = 1.5f;
    float distance = maxDimension * paddingFactor;

    glm::vec3 newCameraPosition = aabbCenter + camera->getZAxis() * distance;
    camera->setPosition(newCameraPosition);
    camera->lookAt(aabbCenter);

    if (auto* easyCam = dynamic_cast<ofEasyCam*>(camera)) {
        easyCam->setTarget(aabbCenter);
    }
}

std::string CameraManager::generateUniqueName(const std::string& baseName) {
    std::string uniqueName = baseName;
    int suffix = 1;

    while (std::any_of(cameras.begin(), cameras.end(), [&](const CameraData& camData) {
        return camData.name == uniqueName;
        })) {
        uniqueName = baseName + std::to_string(suffix++);
    }

    return uniqueName;
}

void CameraManager::setSelectedCamera(int index) {
    if (index >= 0 && index < cameras.size()) {
        if (selectedCameraIndex >= 0 && selectedCameraIndex < cameras.size()) {
            setMouseInput(cameras[selectedCameraIndex].camera, false);
        }

        selectedCameraIndex = index;

        setMouseInput(cameras[selectedCameraIndex].camera, true);
    }
}

ofCamera* CameraManager::getSelectedCamera() {
    if (selectedCameraIndex >= 0 && selectedCameraIndex < cameras.size()) {
        return cameras[selectedCameraIndex].camera;
    }
    return nullptr;
}

void CameraManager::setMouseInput(ofCamera* camera, bool enable) {
    if (auto* easyCam = dynamic_cast<ofEasyCam*>(camera)) {
        if (enable) {
            easyCam->enableMouseInput();
        }
        else {
            easyCam->disableMouseInput();
        }
    }
    else if (auto* freeFlightCam = dynamic_cast<FreeFlightCamera*>(camera)) {
        if (enable) {
            freeFlightCam->enableMouseInput();
        }
        else {
            freeFlightCam->disableMouseInput();
        }
    }
}

void CameraManager::enableSelectedMouseInput() {
    ofCamera* selectedCamera = getSelectedCamera();

    for (auto& camData : cameras) {
        setMouseInput(camData.camera, camData.camera == selectedCamera);
    }
}

void CameraManager::disableAllMouseInput() {
    for (auto& camData : cameras) {
        setMouseInput(camData.camera, false);
    }
}

ofCamera* CameraManager::getFrustumCamera() {
    if (frustumCameraIndex >= 0 && frustumCameraIndex < cameras.size()) {
        return cameras[frustumCameraIndex].camera;
    }
    return nullptr;
}

bool CameraManager::isFrustumCullingEnabled() const {
    return frustumCullingEnabled;
}
