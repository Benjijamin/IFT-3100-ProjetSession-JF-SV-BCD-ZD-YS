/**
 * \file light.h
 * \brief Implémentation des quatre types de lumière directionnelle
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */


#pragma once
#include "ofMain.h"
#include "materials.h"

// Point Light
// Spot Light
// Ambient Light
// Directional Light
enum class LightType {
	Directional,
	Point,
	Spot,
	Ambient
};

class Light {
public:
	// Types de lumière, à intégrer dans un enum avant le push final
	ofColor light_ambient;

	ofLight light_directional;
	ofLight light_point;
	ofLight light_spot;

	ofVec3f getLightPosition(LightType type) const;

	// Pour utiliser getGlobalPosition() dans Shaders, il utiliser des accesseurs pour chacune des lumières
	ofVec3f getDirectionalLightPosition() const;
	ofVec3f getPointLightPosition() const;
	ofVec3f getSpotLightPosition() const;


	// Matériaux, pas vraiment nécessaire puisqu'on gère les matériaux dans le renderer
	void setMaterials(Materials* materials);
	ofMaterial& getCubeMaterial();
	ofMaterial& getSphereMaterial();

	//ofMaterial material_cube;
	//ofMaterial material_sphere;

	// Rotation en quaternions
	ofQuaternion orientation_directional;
	ofQuaternion orientation_spot;

	// Position des primitives
	ofVec3f position_cube;
	ofVec3f position_sphere;

	float camera_offset;

	float oscillation;

	float scale_cube;
	float scale_sphere;
	float scale_teapot;

	// Gerer dans Class Renderer, à retirer avant push final
	// ---------------------------------- //
	float center_x;
	float center_y;

	float speed_motion;

	float oscillation_frequency;
	float oscillation_amplitude;

	float offset_x;
	float offset_z;

	float delta_x;
	float delta_z;

	float initial_x;
	float initial_z;

	// ---------------------------------- //

	bool is_active_ligh_ambient;
	bool is_active_light_directional;
	bool is_active_light_point;
	bool is_active_light_spot;

	bool use_smooth_lighting;

	Light();

	void setup();
	void update();
	void draw();

	void reset();

	void lighting_on();
	void lighting_off();

	float oscillate(float time, float frequency, float amplitude);
	
private:
	Materials* materials_ptr = nullptr;
	std::string cube_material_name = "default";
	std::string sphere_material_name = "default";
};