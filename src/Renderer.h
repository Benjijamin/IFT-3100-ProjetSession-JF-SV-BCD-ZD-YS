/**
 * \file renderer.h
 * \brief Classe regroupant toutes les instantes d'objets modifiant le rendu
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */


#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Material.h"

 // énumération des types de shader
enum class ShaderType { color_fill, lambert, gouraud, phong, blinn_phong, toon };

class Renderer {
public:

	// Gestion des shaders
	ShaderType shader_active;

	ofShader shader_color_fill;
	ofShader shader_lambert;
	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinn_phong;
	ofShader shader_toon;

	ofShader* shader;
	string shader_name;

	// Gestion des types de lumière
	ofColor light_ambient;

	ofLight light_directional;
	ofLight light_point;
	ofLight light_spot;

	ofLight light;

	ofQuaternion orientation_directional;
	ofQuaternion orientation_spot;

	// gestion des matériaux, sera pris en charge par la classe plus tard
	ofMaterial material_cube;
	ofMaterial material_sphere;
	ofMaterial material_chair;

	// Gestion des modèles tests
	ofxAssimpModelLoader chair;

	ofVec3f position_cube;
	ofVec3f position_sphere;
	ofVec3f position_chair;


	float camera_offset;

	float oscillation;

	float scale_cube;
	float scale_sphere;
	float scale_chair;

	float oscillation_frequency;
	float oscillation_amplitude;

	float speed_motion;

	float offset_x;
	float offset_z;

	float delta_x;
	float delta_z;

	float initial_x;
	float initial_z;

	float center_x;
	float center_y;


	bool is_active_light_ambient;
	bool is_active_light_directional;
	bool is_active_light_point;
	bool is_active_light_spot;

	bool use_smooth_lighting;

	Renderer();

	void setup();
	void update();
	void draw();

	void reset();

	void lighting_on();
	void lighting_off();

	void shaderColorFill() const;
	void shaderLambert() const;
	void shaderGouraud() const;
	void shaderPhong() const;
	void shaderBlinnPhong() const;
	void shaderToon() const;

	void shaderAttributes() const;
	void shaderAttributesDynamicM() const;

	float oscillate(float time, float frequency, float amplitude);

	// liaison vers la bibliothèque de matériaux
	void setMaterial(Materials* m) { materials = m; }

	// choisir le matériau courant par son nom
	void setCurrentMaterial(const std::string& name) { currentMaterialName = name; }

private:
	Materials* materials = nullptr;
	std::string currentMaterialName = "default";
};

