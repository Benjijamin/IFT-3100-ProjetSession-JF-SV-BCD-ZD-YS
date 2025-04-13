/**
 * \file light.cpp
 * \brief Implémentation des quatre types de lumière directionnelle
 * \author Yohan
 * \version 0.1
 * \date Mars 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#include "Light.h"

void Light::setup(LightType type_) {
    type = type_;
    light.setup();

    switch (type) {
    case LightType::Directional:
        light.setDirectional();
        light.setDiffuseColor(ofColor::white);
        light.setSpecularColor(ofColor::white);
        break;

    case LightType::Point:
        light.setPointLight();
        light.setDiffuseColor(ofColor::orange);
        break;

    case LightType::Spot:
        light.setSpotlight();
        light.setSpotlightCutOff(45);
        light.setDiffuseColor(ofColor::cyan);
        break;

    case LightType::Ambient:
        // géré via shader ou ofMaterial (à décider plus tard)
        break;
    }
}

void Light::update() {
    // transformer la lumière
}

void Light::draw() {
    light.draw(); // montre une petite sphère à sa position
}

void Light::setPosition(const ofVec3f& pos) {
    light.setPosition(pos);
}

void Light::setDirection(const ofVec3f& dir) {
    light.lookAt(dir);
}

ofVec3f Light::getGlobalPosition() const {
    return light.getGlobalPosition();
}

// fonction d'oscillation
float Light::oscillate(float time, float frequency, float amplitude) {
    return sinf(time * 2.0f * PI / frequency) * amplitude;
}
