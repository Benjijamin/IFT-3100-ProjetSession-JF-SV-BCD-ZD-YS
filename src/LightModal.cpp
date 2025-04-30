#include "LightModal.h"

LightModal::LightModal()
    : isOpenState(false) {
    resetProperties();
}

void LightModal::resetProperties() {
    ambientColor = ofFloatColor(1.0f, 1.0f, 1.0f);
    diffuseColor = ofFloatColor(1.0f, 1.0f, 1.0f);
    specularColor = ofFloatColor(1.0f, 1.0f, 1.0f);

    cutoffAngle = 45.0f;
    spotConcentration = 1.0f;

    constantAttenuation = 1.0f;
    linearAttenuation = 0.0f;
    quadraticAttenuation = 0.0f;

    memset(name, 0, sizeof(name));
}

void LightModal::openNewLightModal(LightType lightType) {
    activeLightType = lightType;
    isOpenState = true;

    std::string initialName = "New " + lightTypeToString(lightType);
    strncpy(name, initialName.c_str(), sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    modalTitle = "Create New Light";

    ImGui::OpenPopup(modalTitle.c_str());
}

void LightModal::openEditLightModal(std::shared_ptr<ofLight> light) {
    isOpenState = true;

    modalTitle = "Edit Light";

    ImGui::OpenPopup(modalTitle.c_str());
}

void LightModal::createNewLight(SceneGraph& sceneGraph) {
    if (!isOpenState) return;

    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0));

    if (ImGui::BeginPopupModal(modalTitle.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Create %s", lightTypeToString(activeLightType).c_str());
        ImGui::InputText("Name", name, sizeof(name));

        ImGui::ColorEdit3("Ambient Color", &ambientColor.r);

        if (activeLightType == LightType::Point || activeLightType == LightType::Directional|| activeLightType == LightType::Spot) {
            ImGui::ColorEdit3("Diffuse Color", &diffuseColor.r);
            ImGui::ColorEdit3("Specular Color", &specularColor.r);
        }

        if (activeLightType == LightType::Spot) {
            ImGui::SliderFloat("Cutoff Angle", &cutoffAngle, 0.0f, 90.0f);
            ImGui::SliderFloat("Spot Concentration", &spotConcentration, 0.0f, 128.0f);
        }

        if (activeLightType == LightType::Point) {
            ImGui::SliderFloat("Constant Attenuation", &constantAttenuation, 0.0f, 1.0f);
            ImGui::SliderFloat("Linear Attenuation", &linearAttenuation, 0.0f, 1.0f);
            ImGui::SliderFloat("Quadratic Attenuation", &quadraticAttenuation, 0.0f, 1.0f);
        }

        if (ImGui::Button("Create")) {
            std::shared_ptr<ofLight> newLight = std::make_shared<ofLight>();

            configureLight(newLight);

            if (newLight) {
                sceneGraph.addLightNode(newLight, std::string(name), sceneGraph.getRootNode());
            }

            resetProperties();
            closeModal();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            closeModal();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
}

void LightModal::editLight(std::shared_ptr<ofLight> light) {
    if (!isOpenState || light == nullptr) return;

    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0));

    if (ImGui::BeginPopupModal(modalTitle.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Edit Light Properties");

        ImVec4 ambient = light->getAmbientColor();
        if (ImGui::ColorEdit3("Ambient Color", (float*)&ambient)) {
            light->setAmbientColor(ofFloatColor(ambient.x, ambient.y, ambient.z));
        }

        if (light->getIsPointLight() || light->getIsDirectional() || light->getIsSpotlight()) {
            ImVec4 diffuse = light->getDiffuseColor();
            if (ImGui::ColorEdit3("Diffuse Color", (float*)&diffuse)) {
                light->setDiffuseColor(ofFloatColor(diffuse.x, diffuse.y, diffuse.z));
            }

            ImVec4 specular = light->getSpecularColor();
            if (ImGui::ColorEdit3("Specular Color", (float*)&specular)) {
                light->setSpecularColor(ofFloatColor(specular.x, specular.y, specular.z));
            }
        }

        if (light->getIsSpotlight()) {
            float cutoffAngle = light->getSpotlightCutOff();
            if (ImGui::SliderFloat("Cutoff Angle", &cutoffAngle, 0.0f, 90.0f)) {
                light->setSpotlightCutOff(cutoffAngle);
            }

            float spotConcentration = light->getSpotConcentration();
            if (ImGui::SliderFloat("Spot Concentration", &spotConcentration, 0.0f, 128.0f)) {
                light->setSpotConcentration(spotConcentration);
            }
        }

        if (light->getIsPointLight()) {
            float constantAttenuation = light->getAttenuationConstant();
            float linearAttenuation = light->getAttenuationLinear();
            float quadraticAttenuation = light->getAttenuationQuadratic();

            if (ImGui::SliderFloat("Constant Attenuation", &constantAttenuation, 0.0f, 1.0f) ||
                ImGui::SliderFloat("Linear Attenuation", &linearAttenuation, 0.0f, 1.0f) ||
                ImGui::SliderFloat("Quadratic Attenuation", &quadraticAttenuation, 0.0f, 1.0f)) {
                light->setAttenuation(constantAttenuation, linearAttenuation, quadraticAttenuation);
            }

        }

        if (ImGui::Button("Close")) {
            closeModal();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
}

void LightModal::closeModal() {
    resetProperties();
    isOpenState = false;
    ImGui::CloseCurrentPopup();
}

bool LightModal::isOpen() const {
    return isOpenState;
}

void LightModal::configureLight(std::shared_ptr<ofLight> light) const {
    light->enable();

    switch (activeLightType) {
    case LightType::Ambient:
        configureAmbientLight(light);
        break;
    case LightType::Point:
        configurePointLight(light);
        break;
    case LightType::Directional:
        configureDirectionalLight(light);
        break;
    case LightType::Spot:
        configureSpotLight(light);
        break;
    }
}

void LightModal::configureAmbientLight(std::shared_ptr<ofLight> light) const {
    light->setAmbientColor(ambientColor);
}

void LightModal::configurePointLight(std::shared_ptr<ofLight> light) const {
    light->setPointLight();
    light->setAmbientColor(ambientColor);
    light->setDiffuseColor(diffuseColor);
    light->setSpecularColor(specularColor);
    light->setAttenuation(constantAttenuation, linearAttenuation, quadraticAttenuation);
}

void LightModal::configureDirectionalLight(std::shared_ptr<ofLight> light) const {
    light->setDirectional();
    light->setAmbientColor(ambientColor);
    light->setDiffuseColor(diffuseColor);
    light->setSpecularColor(specularColor);
}

void LightModal::configureSpotLight(std::shared_ptr<ofLight> light) const {
    light->setSpotlight();
    light->setAmbientColor(ambientColor);
    light->setDiffuseColor(diffuseColor);
    light->setSpecularColor(specularColor);
    light->setSpotlightCutOff(cutoffAngle);
    light->setSpotConcentration(spotConcentration);
}

std::string LightModal::lightTypeToString(LightType lightType) const {
    switch (lightType) {
    case LightType::Ambient: return "Ambient Light";
    case LightType::Point: return "Point Light";
    case LightType::Directional: return "Directional Light";
    case LightType::Spot: return "Spot Light";
    default: return "Unknown Light";
    }
}
