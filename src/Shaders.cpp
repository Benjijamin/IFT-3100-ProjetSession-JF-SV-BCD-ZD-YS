#include "Shaders.h"

void Shaders::setup() {
    listShaderFiles();

    // Paramètres dans le code du prof:
    ofSetFrameRate(60);
    ofSetSphereResolution(32);
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofSetLogLevel(OF_LOG_VERBOSE);


    ofEnableDepthTest();
    ofSetSmoothLighting(true);

    // Configurer la lumiere directionnelle
    dirLight.setup();
    dirLight.setDirectional();
    dirLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    dirLight.setSpecularColor(ofFloatColor(0.1, 0.1, 0.1));
    material.setShininess(128);
    lightDirection = ofVec3f(1.0, -1.0, -1.0); // direction vers la scène
    dirLight.setOrientation(lightDirection);

    // chargement d'un modèle 3D
    teapot.loadModel("geometry/teapot.obj");

    // désactiver le matériau par défaut du modèle
    teapot.disableMaterials();

    // Materials
    material.setDiffuseColor(ofColor(180, 100, 100));
    //material.setShininess(32);

    // Preparer un objet sphere pour les tests
    sphere.setRadius(200);
    sphere.setPosition(0, 0, 0);

    // Chargerement des shaders
    lambertShader.load("shaders/lambert");
    phongShader.load("shaders/phong");
    gouraudShader.load("shaders/gouraud");
    blinn_phongShader.load("shaders/blinn_phong");
    toonShader.load("shaders/toon");

    logShaderStatus();

    activeShader = ShaderType::SHADER_LAMBERT;

    reset();
}

void Shaders::reset() {

}

void Shaders::update() {
    dirLight.setOrientation(lightDirection);
    dirLight.rotate(2, ofVec3f(0, 1, 0));
}

void Shaders::draw() {
    switch (activeShader) {
    case SHADER_LAMBERT:
        drawLambertShader();
        //ofLog() << "drawLambertShader() appellee";
        break;
    case SHADER_PHONG:
        drawPhongShader();
        break;
    case SHADER_GOURAUD:
        drawGouraudShader();
        break;
    case SHADER_BLINN_PHONG:
        drawBlinn_phongShader();
        break;
    case SHADER_TOON:
        drawToonShader();
        break;
    }
}


void Shaders::drawShadersMenu() {
    ImGui::Text("Selection du shader");

    int shaderIndex = static_cast<int>(activeShader);
    if (ImGui::RadioButton("Lambert", shaderIndex == SHADER_LAMBERT)) shaderIndex = SHADER_LAMBERT;
    if (ImGui::RadioButton("Phong", shaderIndex == SHADER_PHONG)) shaderIndex = SHADER_PHONG;
    if (ImGui::RadioButton("Gouraud", shaderIndex == SHADER_GOURAUD)) shaderIndex = SHADER_GOURAUD;
    if (ImGui::RadioButton("Blinn-Phong", shaderIndex == SHADER_BLINN_PHONG)) shaderIndex = SHADER_BLINN_PHONG;
    if (ImGui::RadioButton("Cel Shading (Toon)", shaderIndex == SHADER_TOON)) shaderIndex = SHADER_TOON;

    activeShader = static_cast<ShaderType>(shaderIndex);
}

void Shaders::drawLambertShader() {
    dirLight.enable();
    lambertShader.begin();

    ofFloatColor diffuse = material.getDiffuseColor();
    lambertShader.setUniform3f("ambientColor", 0.05f, 0.05f, 0.05f);
    lambertShader.setUniform3f("diffuseColor", diffuse.r, diffuse.g, diffuse.b);
    lambertShader.setUniform3f("lightDir", -lightDirection.getNormalized());

    drawSphere();

    lambertShader.end();
    dirLight.disable();
}

void Shaders::drawPhongShader() {
    dirLight.enable();
    phongShader.begin();

    ofFloatColor diff = material.getDiffuseColor();
    ofFloatColor spec = material.getSpecularColor();
    ofFloatColor ambient = material.getAmbientColor();

    phongShader.setUniform3f("lightDir", -lightDirection.getNormalized());
    phongShader.setUniform3f("diffuseColor", diff.r, diff.g, diff.b);
    phongShader.setUniform3f("specularColor", spec.r, spec.g, spec.b);
    phongShader.setUniform3f("ambientColor", ambient.r, ambient.g, ambient.b);
    phongShader.setUniform1f("shininess", material.getShininess());

    drawSphere();

    phongShader.end();
    dirLight.disable();
}

void Shaders::drawGouraudShader() {
    dirLight.enable();
    gouraudShader.begin();

    ofFloatColor diff = material.getDiffuseColor();
    ofFloatColor spec = material.getSpecularColor();
    ofFloatColor ambient = material.getAmbientColor();

    gouraudShader.setUniform3f("lightDir", -lightDirection.getNormalized());
    gouraudShader.setUniform3f("diffuseColor", diff.r, diff.g, diff.b);
    gouraudShader.setUniform3f("specularColor", spec.r, spec.g, spec.b);
    gouraudShader.setUniform3f("ambientColor", ambient.r, ambient.g, ambient.b);
    gouraudShader.setUniform1f("shininess", material.getShininess());

    drawSphere();

    gouraudShader.end();
    dirLight.disable();
}

void Shaders::drawBlinn_phongShader() {
    dirLight.enable();
    blinn_phongShader.begin();

    ofFloatColor diff = material.getDiffuseColor();
    ofFloatColor spec = material.getSpecularColor();
    ofFloatColor ambient = material.getAmbientColor();

    blinn_phongShader.setUniform3f("lightDir", -lightDirection.getNormalized());
    blinn_phongShader.setUniform3f("diffuseColor", diff.r, diff.g, diff.b);
    blinn_phongShader.setUniform3f("specularColor", spec.r, spec.g, spec.b);
    blinn_phongShader.setUniform3f("ambientColor", ambient.r, ambient.g, ambient.b);
    blinn_phongShader.setUniform1f("shininess", material.getShininess());

    drawSphere();

    blinn_phongShader.end();
    dirLight.disable();
}

void Shaders::drawToonShader() {
    dirLight.enable();
    toonShader.begin();

    ofFloatColor color = material.getDiffuseColor();

    toonShader.setUniform3f("lightDir", -lightDirection.getNormalized());
    toonShader.setUniform3f("baseColor", color.r, color.g, color.b);

    drawSphere();

    toonShader.end();
    dirLight.disable();
}

void Shaders::listShaderFiles() {
    ofDirectory dir("shaders");
    dir.allowExt("vert");
    dir.allowExt("frag");
    dir.listDir();

    ofLog() << "=== [FICHIERS .vert et .frag dans /shaders/] ===";
    for (auto& file : dir.getFiles()) {
        ofLogNotice() << "📄 " << file.getFileName();
    }
    ofLog() << "=== [Fin de la liste] ===";
}

void Shaders::logShaderStatus() {
    ofLog() << "[SHADER STATUS]";

    auto checkShader = [](const std::string& name, ofShader& shader) {
        if (shader.isLoaded()) {
            ofLogNotice() << "Shader " << name << " load successfully.";
        } else {
            ofLogError() << "Shader " << name << " failed to load.";
        }
    };

    checkShader("Lambert", lambertShader);
    checkShader("Phong", phongShader);
    checkShader("Gouraud", gouraudShader);
    checkShader("Blinn-Phong",blinn_phongShader);
    checkShader("Toon", toonShader);
}

void Shaders::drawSphere() {
    sphere.draw();
}
