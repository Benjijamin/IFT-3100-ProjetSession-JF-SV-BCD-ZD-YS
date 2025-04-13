﻿/**
 * \file shaders.cpp
 * \brief Implémentation des cinq types de shaders
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#include "shaders.h"

ofMaterial& Shaders::getActiveMaterial() {
    // Pour l'instant, toujours utiliser la sphère
    return light->getSphereMaterial();
}

// Méthode qui applique tous les attributs sur un shader
void Shaders::sendMaterialUniforms(ofShader* shader, ofMaterial& mat) {
    shader->setUniform3f("color_ambient", mat.getAmbientColor().r, mat.getAmbientColor().g, mat.getAmbientColor().b);
    shader->setUniform3f("color_diffuse", mat.getDiffuseColor().r, mat.getDiffuseColor().g, mat.getDiffuseColor().b);
    shader->setUniform3f("color_specular", mat.getSpecularColor().r, mat.getSpecularColor().g, mat.getSpecularColor().b);
    shader->setUniform1f("shininess", mat.getShininess());
}

void Shaders::setup() {
    // Settings de la lumiere
    //light = std::make_unique<Light>();
    //light->setup(LightType::Directional); // par defaut
    //light->setup(LightType::Point);
    //light->setup(LightType::Spot);
    //light->setup(LightType::Ambient);

    //light->setPosition(ofVec3f(0, 300, 600));
    //light->setDirection(ofVec3f(0, 0, 0));

    ofSetFrameRate(60);
    ofSetSphereResolution(32);
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofSetLogLevel(OF_LOG_VERBOSE);

    // paramètres
    oscillation_amplitude = 32.0f;
    oscillation_frequency = 7500.0f;
    speed_motion = 150.0f;
    initial_x = 0.0f;
    initial_z = -100.0f;
    scale_cube = 100.0f;
    scale_sphere = 80.0f;
    scale_teapot = 0.618f;

    // initialisation des variables
    offset_x = initial_x;
    offset_z = initial_z;

    delta_x = speed_motion;
    delta_z = speed_motion;

    // chargement d'un modèle 3D
    //teapot.loadModel("geometry/teapot.obj");

    // désactiver le matériau par défaut du modèle
    //teapot.disableMaterials();

    // charger, compiler et linker les sources des shaders
    shader_color_fill.load("shaders/colorFill");
    shader_lambert.load("shaders/lambert");
    shader_gouraud.load("shaders/gouraud");
    shader_phong.load("shaders/phong");
    shader_blinn_phong.load("shaders/blinn_phong");
    shader_toon.load("shaders/toon");

    // shader actif au lancement de la scène
    shader_active = ShaderType::blinn_phong;

    // initialisation de la scène
    reset();
}

void Shaders::reset() {
    // centre du framebuffer
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;

    // caméra à sa position initiale
    offset_x = initial_x;
    offset_z = initial_z;

    // déterminer la position des géométries
    position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
    position_sphere.set(0.0f, 0.0f, 0.0f);
    position_teapot.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);

    ofLog() << "<reset>";
}

void Shaders::update() {
     //transformer la lumière
    //light->setGlobalPosition(
     //   ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
      //  ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
       //-offset_z * 1.5f);

    // mise à jour d'une valeur numérique animée par un oscillateur
    float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;

    // passer les attributs uniformes au shader de sommets
    switch (shader_active) {
    case ShaderType::color_fill:
        shader_name = "Color Fill";
        shader = &shader_color_fill;
        shader->begin();
        shader->setUniform3f("color", 1.0f, 1.0f, 0.0f);
        shader->end();
        break;

    case ShaderType::lambert:
        shader_name = "Lambert";
        shader = &shader_lambert;

        shader->begin();
        sendMaterialUniforms(shader, getActiveMaterial());
        shader->setUniform3f("light_position", light->getLightPosition(LightType::Point));
        shader->end();

        break;

    case ShaderType::gouraud:
        shader_name = "Gouraud";
        shader = &shader_gouraud;

        shader->begin();
        sendMaterialUniforms(shader, getActiveMaterial());
        shader->setUniform3f("light_position", light->getLightPosition(LightType::Point)); // Placeholder, utiliser l'terface pour changer LightType
        shader->setUniform1f("brightness", oscillation);
        shader->end();

        break;

    case ShaderType::phong:
        shader_name = "Phong";
        shader = &shader_phong;
        
        shader->begin();
        sendMaterialUniforms(shader, getActiveMaterial());
        shader->setUniform3f("light_position", light->getLightPosition(LightType::Point)); // Placeholder
        shader->setUniform1f("brightness", oscillation);
        shader->end();

        break;

    case ShaderType::blinn_phong:
        shader_name = "Blinn-Phong";
        shader = &shader_blinn_phong;
        
        shader->begin();
        sendMaterialUniforms(shader, getActiveMaterial());
        shader->setUniform3f("light_position", light->getLightPosition(LightType::Point)); // Placeholder
        shader->setUniform1f("brightness", oscillation);
        shader->end();

        break;

    case ShaderType::toon:
        shader_name = "Toon (Cel Shading)";
        shader = &shader_toon;
        
        shader->begin();
        sendMaterialUniforms(shader, getActiveMaterial());
        shader->setUniform3f("light_position", light->getLightPosition(LightType::Point)); // Placeholder
        shader->setUniform1f("brightness", oscillation);
        shader->end();

        break;

    default:
        break;
    }
}

void Shaders::draw() {
    // activer l'éclairage dynamique
    ofEnableLighting();

    // activer la lumière dynamique
    //light.enable();
    light->draw();

    ofPushMatrix();

    // transformer l'origine de la scène au milieu de la fenêtre d'affichage
    ofTranslate(center_x + offset_x, center_y, offset_z);

    ofPushMatrix();

    // positionnner le cube
    ofTranslate(
        position_cube.x,
        position_cube.y,
        position_cube.z);

    // rotation locale
    ofRotateDeg(45.0f, 1.0f, 0.0f, 0.0f);

    // activer le shader
    shader->begin();

    // dessiner un cube
    ofDrawBox(0.0f, 0.0f, 0.0f, scale_cube);

    ofPopMatrix();

    ofPushMatrix();

    // positionner la sphère
    ofTranslate(
        position_sphere.x,
        position_sphere.y,
        position_sphere.z);

    // dessiner une sphère
    ofDrawSphere(0.0f, 0.0f, 0.0f, scale_sphere);

    ofPopMatrix();

    /*
    ofPushMatrix();

    // positionner le teapot
    teapot.setPosition(
        position_teapot.x,
        position_teapot.y + 15.0f,
        position_teapot.z);

    // dimension du teapot
    teapot.setScale(
        scale_teapot,
        scale_teapot,
        scale_teapot);

    // dessiner un teapot
    teapot.draw(OF_MESH_FILL);

    ofPopMatrix();
    */

    ofPopMatrix();

    // désactiver le shader
    shader->end();

    // désactiver la lumière
    //light.disable();

    // désactiver l'éclairage dynamique
    ofDisableLighting();
}

// fonction d'oscillation
float Shaders::oscillate(float time, float frequency, float amplitude) {
    return sinf(time * 2.0f * PI / frequency) * amplitude;
}
