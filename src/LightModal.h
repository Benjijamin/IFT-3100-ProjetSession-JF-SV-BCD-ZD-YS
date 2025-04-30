#ifndef LIGHT_MODAL_H
#define LIGHT_MODAL_H

#include <memory>
#include <string>
#include "ofMain.h"
#include "ofxImGui.h"
#include "SceneGraph.h"

class LightModal {
public:
    enum class LightType {
        Ambient,
        Point,
        Directional,
        Spot
    };

    LightModal();

    void openNewLightModal(LightType lightType);
    void openEditLightModal(std::shared_ptr<ofLight> light);
    void createNewLight(SceneGraph& sceneGraph);
    void editLight(std::shared_ptr<ofLight> light);
    void closeModal();

    bool isOpen() const;

private:
    void resetProperties();
    void configureLight(std::shared_ptr<ofLight> light) const;

    void configureAmbientLight(std::shared_ptr<ofLight> light) const;
    void configurePointLight(std::shared_ptr<ofLight> light) const;
    void configureDirectionalLight(std::shared_ptr<ofLight> light) const;
    void configureSpotLight(std::shared_ptr<ofLight> light) const;

    std::string lightTypeToString(LightType lightType) const;

    std::string modalTitle;
    bool isOpenState;
    LightType activeLightType;

    char name[128];
    ofFloatColor ambientColor;
    ofFloatColor diffuseColor;
    ofFloatColor specularColor;
    float cutoffAngle;
    float spotConcentration;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

#endif // LIGHT_MODAL_H
