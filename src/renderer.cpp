/**
 * \file renderer.cpp
 * \brief Classe regroupant toutes les instantes d'objets modifiant le rendu
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#include "renderer.h"

Renderer::Renderer() {}

void Renderer::setup() {
    ofSetFrameRate(60);
    ofSetSphereResolution(32);
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofSetLogLevel(OF_LOG_VERBOSE);

    // paramètres
    camera_offset = 350.0f; // à changer
    oscillation_amplitude = 32.0f;
    oscillation_frequency = 7500.0f;
    speed_motion = 150.0f;
    initial_x = 0.0f;
    initial_z = -100.0f;
    scale_cube = 100.0f;
    scale_sphere = 80.0f;
    scale_chair = 0.618f;

    // initialisation des variables
    offset_x = initial_x;
    offset_z = initial_z;

    delta_x = speed_motion;
    delta_z = speed_motion;

    // chargement d'un modèle 3D
    chair.loadModel("geometry/chair_01.obj");

    // désactiver le matériau par défaut du modèle
    chair.disableMaterials();

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

void Renderer::reset() {
    // initialisation des variables
    offset_x = initial_x;
    offset_z = initial_z;

    delta_x = speed_motion;
    delta_z = speed_motion;

    use_smooth_lighting = true;

    is_active_light_ambient = true;
    is_active_light_directional = true;
    is_active_light_point = true;
    is_active_light_spot = true;

    // centre du framebuffer
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;

    // déterminer la position des géométries
    position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
    position_sphere.set(0.0f, 0.0f, 0.0f);
    position_chair.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);

    // configurer le matériau du cube
    material_cube.setAmbientColor(ofColor(63, 63, 63));
    material_cube.setDiffuseColor(ofColor(127, 0, 0));
    material_cube.setEmissiveColor(ofColor(31, 0, 0));
    material_cube.setSpecularColor(ofColor(127, 127, 127));
    material_cube.setShininess(16.0f);

    // configurer le matériau de la sphère
    material_sphere.setAmbientColor(ofColor(63, 63, 63));
    material_sphere.setDiffuseColor(ofColor(191, 63, 0));
    material_sphere.setEmissiveColor(ofColor(0, 31, 0));
    material_sphere.setSpecularColor(ofColor(255, 255, 64));
    material_sphere.setShininess(8.0f);

    // configurer le matériau de la chaise
    material_chair.setAmbientColor(ofColor(63, 63, 63));
    material_chair.setDiffuseColor(ofColor(63, 0, 63));
    material_chair.setEmissiveColor(ofColor(0, 0, 31));
    material_chair.setSpecularColor(ofColor(191, 191, 191));
    material_chair.setShininess(8.0f);

    // configurer la lumière ambiante
    light_ambient.set(30, 30, 30);

    // configurer la lumière directionnelle
    light_directional.setDiffuseColor(ofColor(31, 255, 31));
    light_directional.setSpecularColor(ofColor(191, 191, 191));
    light_directional.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
    light_directional.setDirectional();

    // configurer la lumière ponctuelle
    light_point.setDiffuseColor(ofColor(255, 255, 255));
    light_point.setSpecularColor(ofColor(191, 191, 191));
    light_point.setPointLight();

    // configurer la lumière projecteur
    light_spot.setDiffuseColor(ofColor(191, 191, 191));
    light_spot.setSpecularColor(ofColor(191, 191, 191));
    light_spot.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
    light_spot.setSpotConcentration(2);
    light_spot.setSpotlightCutOff(30);
    light_spot.setSpotlight();

    ofLog() << "<reset>";
}

void Renderer::update() {
    // transformer la lumière
    //light.setGlobalPosition(
    //    ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
    //    ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
    //    -offset_z * 1.5f);

    ofPushMatrix();

    if (is_active_light_directional) {
        // transformer la lumière directionnelle
        orientation_directional.makeRotate(int(ofGetElapsedTimeMillis() * 0.1f) % 360, 0, 1, 0);
        light_directional.setPosition(center_x, center_y + 60, camera_offset * 0.75f); // mettre dynamique plus tard
        light_directional.setOrientation(orientation_directional);
    }

    if (is_active_light_point) {
        // transformer la lumière ponctuelle
        light_point.setPosition(ofGetMouseX(), ofGetMouseY(), camera_offset * 0.75f);
    }

    if (is_active_light_spot) {
        // transformer la lumière projecteur
        oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude);
        orientation_spot.makeRotate(30.0f, ofVec3f(1, 0, 0), oscillation, ofVec3f(0, 1, 0), 0.0f, ofVec3f(0, 0, 1));
        light_spot.setOrientation(orientation_spot);
        light_spot.setPosition(center_x, center_y - 75.0f, camera_offset * 0.75f);
    }

    ofPopMatrix();

    // mise à jour d'une valeur numérique animée par un oscillateur
    //float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;

    // passer les attributs uniformes au shader de sommets
    switch (shader_active) {
    case ShaderType::color_fill:
        shader_name = "Color Fill";
        shader = &shader_color_fill;
        //shader->begin();
        //shader->setUniform3f("color", 1.0f, 1.0f, 0.0f);
        //shader->end();
        break;

    case ShaderType::lambert:
        shader_name = "Lambert";
        shader = &shader_lambert;
        //shader->begin();
        // — Matériau
        //shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        //shader->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
        //shaderLambert();
        //shader->end();
        break;


    case ShaderType::gouraud:
        shader_name = "Gouraud";
        shader = &shader_gouraud;
        //shader->begin();
        //shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        //shader->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
        //shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        //shader->setUniform1f("brightness", oscillation);
        //shader->end();
        break;

    case ShaderType::phong:
        shader_name = "Phong";
        shader = &shader_phong;
        //shader->begin();
        //shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        //shader->setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
        //shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        //shader->setUniform1f("brightness", oscillation);
        //shaderPhong();
        //shader->end();
        break;

    case ShaderType::blinn_phong:
        shader_name = "Blinn-Phong";
        shader = &shader_blinn_phong;
        //shader->begin();
        //shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        //shader->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
        //shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        //shader->setUniform1f("brightness", oscillation);
        //shaderBlinnPhong();
        //shader->end();
        break;

    case ShaderType::toon:
        shader_name = "Cel Shading (toon)";
        shader = &shader_toon;
        //shader->begin();
        //shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        //shader->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
        //shaderToon();
        //shader->end();
        break;

    default:
        break;
    }
}

void Renderer::draw() {
    ofBackground(0);
    //ofEnableLighting(); // activer l'éclairage dynamique
    //lighting_on();
    //light.enable(); // activer la lumière dynamique

    shader->begin(); // activer le shader

    // Shaders disponibles
    switch (shader_active) {
    case ShaderType::color_fill:  shaderColorFill();    break;
    case ShaderType::lambert:     shaderLambert();      break;
    case ShaderType::gouraud:     shaderGouraud();      break;
    case ShaderType::phong:       shaderPhong();        break;
    case ShaderType::blinn_phong: shaderBlinnPhong();   break;
    case ShaderType::toon:        shaderToon();         break;
    }

    ofPushMatrix();
        // afficher un repère visuel pour les lumières
        if (is_active_light_point)
            light_point.draw();
        if (is_active_light_directional)
            light_directional.draw();
        if (is_active_light_spot)
            light_spot.draw();

        ofEnableLighting();
        lighting_on();

        // transformer l'origine de la scène au milieu de la fenêtre d'affichage
        ofTranslate(center_x + offset_x, center_y, offset_z);

        ofPushMatrix();
            ofTranslate(position_cube.x, position_cube.y, position_cube.z); // positionnner le cube
            ofRotateDeg(45.0f, 1.0f, 0.0f, 0.0f); // rotation locale
            ofDrawBox(0.0f, 0.0f, 0.0f, scale_cube); // dessiner un cube
        ofPopMatrix();

        ofPushMatrix();
           ofTranslate(position_sphere.x, position_sphere.y, position_sphere.z); // positionner la sphère
           ofDrawSphere(0.0f, 0.0f, 0.0f, scale_sphere); // dessiner une sphère
        ofPopMatrix();

        ofPushMatrix();
            chair.setPosition(position_chair.x, position_chair.y + 15.0f, position_chair.z); // positionner l'objet (chaise)
            chair.setScale(scale_chair, scale_chair, scale_chair);
            chair.draw(OF_MESH_FILL);
        ofPopMatrix();
    ofPopMatrix();

    shader->end(); // désactiver le shader

    //light.disable(); // désactiver la lumière
    lighting_off();
    ofDisableLighting(); // désactiver l'éclairage dynamique
}

void Renderer::shaderColorFill() const {
    shader->setUniform3f("color", 1.0f, 1.0f, 0.0f);
}
void Renderer::shaderLambert() const {
    shaderAttributes();
}
void Renderer::shaderGouraud() const {
    shaderAttributes();
}
void Renderer::shaderPhong() const {
    shaderAttributes();
}
void Renderer::shaderBlinnPhong() const {
    shaderAttributes();
}
void Renderer::shaderToon() const {
    shaderAttributes();
}

void Renderer::shaderAttributes() const {
    shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
    shader->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
    shader->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
    shader->setUniform1f("brightness", 1.0f);

    // — Lumière ambiante
    shader->setUniform1i("use_light_ambient", is_active_light_ambient);
    if (is_active_light_ambient) {
        // couleur ambiante (0–1)
        shader->setUniform3f("light_ambient",
            light_ambient.r / 255.0f,
            light_ambient.g / 255.0f,
            light_ambient.b / 255.0f);
    }

    // — Lumière directionnelle
    shader->setUniform1i("use_light_directional", is_active_light_directional);
    if (is_active_light_directional) {
        // direction locale Z⁻ (vers la lumière)
        glm::vec3 dir = orientation_directional * glm::vec3(0, 0, -1);
        shader->setUniform3f("light_directional_direction", dir.x, dir.y, dir.z);

        ofFloatColor diffDir = light_directional.getDiffuseColor();
        shader->setUniform3f("light_directional_diffuse", diffDir.r, diffDir.g, diffDir.b);
    }

    // — Lumière ponctuelle
    shader->setUniform1i("use_light_point", is_active_light_point);
    if (is_active_light_point) {
        glm::vec3 posP = light_point.getGlobalPosition();
        glm::mat4 view = ofGetCurrentViewMatrix();
        glm::vec4 P = view * glm::vec4(posP, 1.0f);
        shader->setUniform3f("light_point_position", P.x, P.y, P.z);

        ofFloatColor diffP = light_point.getDiffuseColor();
        shader->setUniform3f("light_point_diffuse", diffP.r, diffP.g, diffP.b);

        // constantes d’atténuation, integrer dans l'interface
        shader->setUniform1f("light_point_constant", 0.5f);
        shader->setUniform1f("light_point_linear", 0.0001);
        shader->setUniform1f("light_point_quadratic", 0.0001f);
    }

    // — Lumière spot
    shader->setUniform1i("use_light_spot", is_active_light_spot);
    if (is_active_light_spot) {
        glm::vec3 posS = light_spot.getGlobalPosition();
        glm::mat4 view = ofGetCurrentViewMatrix();
        glm::vec4 S = view * glm::vec4(posS, 1.0f);
        glm::vec3 dirS = orientation_spot * glm::vec3(0, 0, -1);
        shader->setUniform3f("light_spot_position", S.x, S.y, S.z);
        shader->setUniform3f("light_spot_direction", dirS.x, dirS.y, dirS.z);

        // cut‑off en cosinus
        float cut = glm::cos(glm::radians(light_spot.getSpotlightCutOff()));
        float outer = glm::cos(glm::radians(light_spot.getSpotConcentration()));
        shader->setUniform1f("light_spot_cutoff", cut);
        shader->setUniform1f("light_spot_outerCutoff", outer);

        ofFloatColor diffS = light_spot.getDiffuseColor();
        shader->setUniform3f("light_spot_diffuse", diffS.r, diffS.g, diffS.b);

        // même atténuation que ponctuelle ou spécifique
        shader->setUniform1f("light_spot_constant", 0.5f);
        shader->setUniform1f("light_spot_linear", 0.00001f);
        shader->setUniform1f("light_spot_quadratic", 0.00001f);
    }
}




void Renderer::lighting_on() {
    if (is_active_light_ambient)
        ofSetGlobalAmbientColor(light_ambient);
    else
        ofSetGlobalAmbientColor(ofColor(0, 0, 0));

    if (is_active_light_directional)
        light_directional.enable();

    if (is_active_light_point)
        light_point.enable();

    if (is_active_light_spot)
        light_spot.enable();
}

// activation des lumières dynamiques
void Renderer::lighting_off() {
    ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    light_directional.disable();
    light_point.disable();
    light_spot.disable();
}

// fonction d'oscillation, pour tests seulement
float Renderer::oscillate(float time, float frequency, float amplitude) {
    return sinf(time * 2.0f * PI / frequency) * amplitude;
}