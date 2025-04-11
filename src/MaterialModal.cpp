#include "MaterialModal.h"

MaterialModal::MaterialModal()
    : isOpenState(false), currentPhongMaterialIndex(0), currentPBRMaterialIndex(0) {
    initializeMaterials();
}

void MaterialModal::initializeMaterials() {
    phongMaterialNames.clear();
    phongMaterials.clear();
    pbrMaterialNames.clear();
    pbrMaterials.clear();

    pbrMaterialNames = {
        "Aluminum", "Copper", "Gold", "Steel", "Bronze", "Brass", "Chrome", "Plastic", "Painted Metal", "Rubber"
    };

    auto addPBRMaterial = [&](ofFloatColor diffuse, float metallic, float roughness, float reflectance,
        bool clearCoatEnabled = false, float clearCoatStrength = 0.0f, float clearCoatRoughness = 0.0f) {
            ofMaterial mat;
            mat.setDiffuseColor(diffuse);
            mat.setMetallic(metallic);
            mat.setRoughness(roughness);
            mat.setReflectance(reflectance);
            mat.setClearCoatEnabled(clearCoatEnabled);
            mat.setClearCoatStrength(clearCoatStrength);
            mat.setClearCoatRoughness(clearCoatRoughness);
            pbrMaterials.push_back(mat);
        };

    addPBRMaterial(ofFloatColor(0.77f, 0.77f, 0.77f), 0.91f, 0.35f, 0.7f, true, 0.6f, 0.2f);  // Aluminum
    addPBRMaterial(ofFloatColor(0.95f, 0.64f, 0.54f), 0.95f, 0.25f, 0.85f);                   // Copper
    addPBRMaterial(ofFloatColor(1.0f, 0.84f, 0.0f), 1.0f, 0.4f, 0.9f, true, 0.7f, 0.3f);      // Gold
    addPBRMaterial(ofFloatColor(0.75f, 0.75f, 0.75f), 1.0f, 0.25f, 0.85f, true, 0.6f, 0.2f);  // Steel
    addPBRMaterial(ofFloatColor(0.55f, 0.47f, 0.14f), 0.9f, 0.4f, 0.7f);                      // Bronze
    addPBRMaterial(ofFloatColor(0.78f, 0.57f, 0.11f), 0.85f, 0.5f, 0.8f);                     // Brass
    addPBRMaterial(ofFloatColor(0.8f, 0.8f, 0.8f), 0.95f, 0.2f, 0.9f, true, 0.7f, 0.2f);      // Chrome
    addPBRMaterial(ofFloatColor(0.9f, 0.2f, 0.2f), 0.0f, 0.7f, 0.5f);                         // Plastic
    addPBRMaterial(ofFloatColor(0.9f, 0.2f, 0.2f), 0.9f, 0.35f, 0.85f, true, 0.5f, 0.3f);     // Painted Metal
    addPBRMaterial(ofFloatColor(0.2f, 0.2f, 0.2f), 0.0f, 0.8f, 0.3f);                         // Rubber

    phongMaterialNames = {
        "Emerald", "Jade", "Obsidian", "Pearl", "Ruby", "Turquoise", "Brass", "Bronze",
        "Chrome", "Copper", "Gold", "Silver", "Black Plastic", "Cyan Plastic",
        "Green Plastic", "Red Plastic", "White Plastic", "Yellow Plastic",
        "Black Rubber", "Cyan Rubber", "Green Rubber", "Red Rubber", "White Rubber",
        "Yellow Rubber"
    };

    auto addPhongMaterial = [&](glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
        ofMaterial mat;
        mat.setAmbientColor(ofColor(ambient.x * 255, ambient.y * 255, ambient.z * 255));
        mat.setDiffuseColor(ofColor(diffuse.x * 255, diffuse.y * 255, diffuse.z * 255));
        mat.setSpecularColor(ofColor(specular.x * 255, specular.y * 255, specular.z * 255));
        mat.setShininess(shininess * 128);
        phongMaterials.push_back(mat);
        };

    addPhongMaterial({ 0.0215, 0.1745, 0.0215 }, { 0.07568, 0.61424, 0.07568 }, { 0.633, 0.727811, 0.633 }, 0.6); // Emerald
    addPhongMaterial({ 0.135, 0.2225, 0.1575 }, { 0.54, 0.89, 0.63 }, { 0.316228, 0.316228, 0.316228 }, 0.1); // Jade
    addPhongMaterial({ 0.05375, 0.05, 0.06625 }, { 0.18275, 0.17, 0.22525 }, { 0.332741, 0.328634, 0.346435 }, 0.3); // Obsidian
    addPhongMaterial({ 0.25, 0.20725, 0.20725 }, { 1, 0.829, 0.829 }, { 0.296648, 0.296648, 0.296648 }, 0.088); // Pearl
    addPhongMaterial({ 0.1745, 0.01175, 0.01175 }, { 0.61424, 0.04136, 0.04136 }, { 0.727811, 0.626959, 0.626959 }, 0.6); // Ruby
    addPhongMaterial({ 0.1, 0.18725, 0.1745 }, { 0.396, 0.74151, 0.69102 }, { 0.297254, 0.30829, 0.306678 }, 0.1); // Turquoise
    addPhongMaterial({ 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 }, { 0.992157, 0.941176, 0.807843 }, 0.21794872); // Brass
    addPhongMaterial({ 0.2125, 0.1275, 0.054 }, { 0.714, 0.4284, 0.18144 }, { 0.393548, 0.271906, 0.166721 }, 0.2); // Bronze
    addPhongMaterial({ 0.25, 0.25, 0.25 }, { 0.4, 0.4, 0.4 }, { 0.774597, 0.774597, 0.774597 }, 0.6); // Chrome
    addPhongMaterial({ 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 }, { 0.256777, 0.137622, 0.086014 }, 0.1); // Copper
    addPhongMaterial({ 0.24725, 0.1995, 0.0745 }, { 0.75164, 0.60648, 0.22648 }, { 0.628281, 0.555802, 0.366065 }, 0.4); // Gold
    addPhongMaterial({ 0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 0.4); // Silver
    addPhongMaterial({ 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 }, { 0.50, 0.50, 0.50 }, 0.25); // Black Plastic
    addPhongMaterial({ 0.0, 0.1, 0.06 }, { 0.0, 0.50980392, 0.50980392 }, { 0.50196078, 0.50196078, 0.50196078 }, 0.25); // Cyan Plastic
    addPhongMaterial({ 0.0, 0.0, 0.0 }, { 0.1, 0.35, 0.1 }, { 0.45, 0.55, 0.45 }, 0.25); // Green Plastic
    addPhongMaterial({ 0.0, 0.0, 0.0 }, { 0.5, 0.0, 0.0 }, { 0.7, 0.6, 0.6 }, 0.25); // Red Plastic
    addPhongMaterial({ 0.0, 0.0, 0.0 }, { 0.55, 0.55, 0.55 }, { 0.70, 0.70, 0.70 }, 0.25); // White Plastic
    addPhongMaterial({ 0.0, 0.0, 0.0 }, { 0.5, 0.5, 0.0 }, { 0.60, 0.60, 0.50 }, 0.25); // Yellow Plastic
    addPhongMaterial({ 0.02, 0.02, 0.02 }, { 0.01, 0.01, 0.01 }, { 0.4, 0.4, 0.4 }, 0.078125); // Black Rubber
    addPhongMaterial({ 0.0, 0.05, 0.05 }, { 0.4, 0.5, 0.5 }, { 0.04, 0.7, 0.7 }, 0.078125); // Cyan Rubber
    addPhongMaterial({ 0.0, 0.05, 0.0 }, { 0.4, 0.5, 0.4 }, { 0.04, 0.7, 0.04 }, 0.078125); // Green Rubber
    addPhongMaterial({ 0.05, 0.0, 0.0 }, { 0.5, 0.4, 0.4 }, { 0.7, 0.04, 0.04 }, 0.078125); // Red Rubber
    addPhongMaterial({ 0.05, 0.05, 0.05 }, { 0.5, 0.5, 0.5 }, { 0.7, 0.7, 0.7 }, 0.078125); // White Rubber
    addPhongMaterial({ 0.05, 0.05, 0.0 }, { 0.5, 0.5, 0.4 }, { 0.7, 0.7, 0.04 }, 0.078125); // Yellow Rubber
}

void MaterialModal::openModal(std::shared_ptr<ofMaterial> material) {
    activeMaterial = material;

    isOpenState = true;
    modalTitle = "Edit Material";
    ImGui::OpenPopup(modalTitle.c_str());
}

void MaterialModal::closeModal() {
    isOpenState = false;
    ImGui::CloseCurrentPopup();
}

void MaterialModal::draw() {
    if (!isOpenState) return;

    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0));

    if (ImGui::BeginPopupModal(modalTitle.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::Checkbox("Use PBR Workflow", &usePBRWorkflow)) {
            activeMaterial->setPBR(usePBRWorkflow);
        }

        drawMaterialDropdown();
        drawMaterialProperties();

        if (ImGui::Button("Close")) {
            closeModal();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
}

void MaterialModal::drawMaterialDropdown() {
    if (usePBRWorkflow) {
        if (ImGui::Combo("Select Material (PBR)", &currentPBRMaterialIndex,
            [](void* data, int idx, const char** out_text) {
                *out_text = ((std::vector<std::string>*)data)->at(idx).c_str();
                return true;
            },
            (void*)&pbrMaterialNames,
            pbrMaterialNames.size())) {
            if (currentPBRMaterialIndex >= 0 && currentPBRMaterialIndex < pbrMaterials.size() && activeMaterial) {
                *activeMaterial = pbrMaterials[currentPBRMaterialIndex];
            }
        }
    }
    else {
        if (ImGui::Combo("Select Material (Phong)", &currentPhongMaterialIndex,
            [](void* data, int idx, const char** out_text) {
                *out_text = ((std::vector<std::string>*)data)->at(idx).c_str();
                return true;
            },
            (void*)&phongMaterialNames,
            phongMaterialNames.size())) {
            if (currentPhongMaterialIndex >= 0 && currentPhongMaterialIndex < phongMaterials.size() && activeMaterial) {
                *activeMaterial = phongMaterials[currentPhongMaterialIndex];
            }
        }
    }
}

void MaterialModal::drawMaterialProperties() {
    ImGui::Text("Material Properties");

    if (activeMaterial) {
        if (usePBRWorkflow) {
            ImVec4 albedo = activeMaterial->getDiffuseColor();
            if (ImGui::ColorEdit3("Albedo", (float*)&albedo)) {
                activeMaterial->setDiffuseColor(ofFloatColor(albedo.x, albedo.y, albedo.z));
            }

            float metallic = activeMaterial->getMetallic();
            if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f)) {
                activeMaterial->setMetallic(metallic);
            }

            float roughness = activeMaterial->getRoughness();
            if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
                activeMaterial->setRoughness(roughness);
            }

            float reflectance = activeMaterial->getReflectance();
            if (ImGui::SliderFloat("Reflectance", &reflectance, 0.0f, 1.0f)) {
                activeMaterial->setReflectance(reflectance);
            }

            bool clearCoatEnabled = activeMaterial->isClearCoatEnabled();
            if (ImGui::Checkbox("Enable Clear Coat", &clearCoatEnabled)) {
                activeMaterial->setClearCoatEnabled(clearCoatEnabled);
            }

            if (clearCoatEnabled) {
                float clearCoatStrength = activeMaterial->getClearCoatStrength();
                if (ImGui::SliderFloat("Clear Coat Strength", &clearCoatStrength, 0.0f, 1.0f)) {
                    activeMaterial->setClearCoatStrength(clearCoatStrength);
                }

                float clearCoatRoughness = activeMaterial->getClearCoatRoughness();
                if (ImGui::SliderFloat("Clear Coat Roughness", &clearCoatRoughness, 0.0f, 1.0f)) {
                    activeMaterial->setClearCoatRoughness(clearCoatRoughness);
                }
            }
        }
        else {
            ImVec4 diffuse = activeMaterial->getDiffuseColor();
            if (ImGui::ColorEdit3("Diffuse Color", (float*)&diffuse)) {
                activeMaterial->setDiffuseColor(ofFloatColor(diffuse.x, diffuse.y, diffuse.z));
            }

            ImVec4 ambient = activeMaterial->getAmbientColor();
            if (ImGui::ColorEdit3("Ambient Color", (float*)&ambient)) {
                activeMaterial->setAmbientColor(ofFloatColor(ambient.x, ambient.y, ambient.z));
            }

            ImVec4 specular = activeMaterial->getSpecularColor();
            if (ImGui::ColorEdit3("Specular Color", (float*)&specular)) {
                activeMaterial->setSpecularColor(ofFloatColor(specular.x, specular.y, specular.z));
            }

            ImVec4 emissive = activeMaterial->getEmissiveColor();
            if (ImGui::ColorEdit3("Emissive Color", (float*)&emissive)) {
                activeMaterial->setEmissiveColor(ofFloatColor(emissive.x, emissive.y, emissive.z));
            }

            float shininess = activeMaterial->getShininess();
            if (ImGui::SliderFloat("Shininess", &shininess, 0.0f, 128.0f)) {
                activeMaterial->setShininess(shininess);
            }
        }
    }
}

bool MaterialModal::isOpen() const {
    return isOpenState;
}

std::shared_ptr<ofMaterial> MaterialModal::getMaterial() const {
    return activeMaterial;
}
