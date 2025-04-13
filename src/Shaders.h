/**
 * \file shaders.h
 * \brief Implémentation des cinq types de shaders
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#pragma once

#include "ofMain.h"
#include "light.h"
#include "materials.h"
#include "ofxAssimpModelLoader.h"

// énumération des types de shader
enum class ShaderType { color_fill, lambert, gouraud, phong, blinn_phong, toon };

class Shaders {
public:
	ofMaterial& getActiveMaterial();
	void sendMaterialUniforms(ofShader* shader, ofMaterial& mat);

	ShaderType shader_active;

	ofShader shader_color_fill;
	ofShader shader_lambert;
	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinn_phong;
	ofShader shader_toon;

	ofShader* shader;

	Light light; // objet de type Light

	//ofxAssimpModelLoader teapot;

	ofVec3f position_cube;
	ofVec3f position_sphere;
	ofVec3f position_teapot;

	string shader_name;
	string lightName;

	float oscillation;

	float scale_cube;
	float scale_sphere;
	float scale_teapot;

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

	void setup();
	void update();
	void draw();
	void reset();

	float oscillate(float time, float frequency, float amplitude);

private:
	std::unique_ptr<Light> light;
};
