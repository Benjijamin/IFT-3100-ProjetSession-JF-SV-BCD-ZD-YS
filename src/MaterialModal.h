#ifndef MATERIAL_MODAL_H
#define MATERIAL_MODAL_H

#include "ofMain.h"
#include "ofxImGui.h"

class MaterialModal {
public:
    MaterialModal();

    void openModal(std::shared_ptr<ofMaterial> material);
    void closeModal();
    void draw();

    bool isOpen() const;
    std::shared_ptr<ofMaterial> getMaterial() const;

private:
    void initializeMaterials();
    void drawMaterialProperties();
    void drawMaterialDropdown();

    bool isOpenState;
    std::string modalTitle;
    std::shared_ptr<ofMaterial> activeMaterial;

    std::vector<std::string> phongMaterialNames;
    std::vector<ofMaterial> phongMaterials;
    int currentPhongMaterialIndex;

    std::vector<std::string> pbrMaterialNames;
    std::vector<ofMaterial> pbrMaterials;
    int currentPBRMaterialIndex;

    bool usePBRWorkflow;
};

#endif // MATERIAL_MODAL_H
