/**
 * \file light.cpp
 * \brief Impl�mentation des quatre types de lumi�re directionnelle
 * \author Yohan
 * \version 0.1
 * \date Mars 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#include "Light.h"

ofVec3f Light::getLightPosition(LightType type) const {
	switch (type) {
	case LightType::Directional:
		return getDirectionalLightPosition();
	case LightType::Point:
		return getPointLightPosition();
	case LightType::Spot:
		return getSpotLightPosition();
	default:
		return ofVec3f(0, 0, 0); //Ambient n�a pas de position
	}
}

ofVec3f Light::getDirectionalLightPosition() const {
	return light_directional.getGlobalPosition();
}

ofVec3f Light::getPointLightPosition() const {
	return light_point.getGlobalPosition();
}

ofVec3f Light::getSpotLightPosition() const {
	return light_spot.getGlobalPosition();
}

void Light::setMaterials(Materials* materials) {
	materials_ptr = materials;

	// Cr�er ou assigner les mat�riaux n�cessaires
	ofMaterial cubeMat;
	cubeMat.setAmbientColor(ofColor(63, 63, 63));
	cubeMat.setDiffuseColor(ofColor(127, 0, 0));
	cubeMat.setEmissiveColor(ofColor(31, 0, 0));
	cubeMat.setSpecularColor(ofColor(127, 127, 127));
	cubeMat.setShininess(16.0f);

	ofMaterial sphereMat;
	sphereMat.setAmbientColor(ofColor(63, 63, 63));
	sphereMat.setDiffuseColor(ofColor(191, 63, 0));
	sphereMat.setEmissiveColor(ofColor(0, 31, 0));
	sphereMat.setSpecularColor(ofColor(255, 255, 64));
	sphereMat.setShininess(8.0f);

	materials_ptr->createMaterial("cube_mat", cubeMat);
	materials_ptr->createMaterial("sphere_mat", sphereMat);

	cube_material_name = "cube_mat";
	sphere_material_name = "sphere_mat";
}

ofMaterial& Light::getCubeMaterial() {
	return materials_ptr->getMaterial(cube_material_name);
}

ofMaterial& Light::getSphereMaterial() {
	return materials_ptr->getMaterial(sphere_material_name);
}

void Light::setup() {
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofSetSphereResolution(32);

	// param�tres
	camera_offset = 350.0f;
	speed_motion = 150.0f;
	oscillation_frequency = 7500.0f;
	oscillation_amplitude = 45.0;
	initial_x = 0.0f;
	initial_z = -100.0f;
	scale_cube = 100.0f;
	scale_sphere = 80.0f;

	// initialisation des params de la scene
	reset();
}

void Light::reset() {
	// initialisation des variables
	offset_x = initial_x;
	offset_z = initial_z;

	delta_x = speed_motion;
	delta_z = speed_motion;

	use_smooth_lighting = true;

	is_active_ligh_ambient = true;
	is_active_light_directional = true;
	is_active_light_point = true;
	is_active_light_spot = true;

	// centre du framebuffer
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	// d�terminer la position des g�om�tries
	position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	position_sphere.set(0.0f, 0.0f, 0.0f);

	// G�rer par la classe Materials
	/*
	// configurer le mat�riau du cube, sera trait� par shader
	material_cube.setAmbientColor(ofColor(63, 63, 63));
	material_cube.setDiffuseColor(ofColor(127, 0, 0));
	material_cube.setEmissiveColor(ofColor(31, 0, 0));
	material_cube.setSpecularColor(ofColor(127, 127, 127));
	material_cube.setShininess(16.0f);

	// configurer le mat�riau de la sph�re
	material_sphere.setAmbientColor(ofColor(63, 63, 63));
	material_sphere.setDiffuseColor(ofColor(191, 63, 0));
	material_sphere.setEmissiveColor(ofColor(0, 31, 0));
	material_sphere.setSpecularColor(ofColor(255, 255, 64));
	material_sphere.setShininess(8.0f);
	*/


	// configurer la lumi�re ambiante
	light_ambient.set(127, 127, 127);

	// configurer la lumi�re directionnelle
	light_directional.setDiffuseColor(ofColor(31, 255, 31));
	light_directional.setSpecularColor(ofColor(191, 191, 191));
	light_directional.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_directional.setDirectional();

	// configurer la lumi�re ponctuelle
	light_point.setDiffuseColor(ofColor(255, 255, 255));
	light_point.setSpecularColor(ofColor(191, 191, 191));
	light_point.setPointLight();

	// configurer la lumi�re projecteur
	light_spot.setDiffuseColor(ofColor(191, 191, 191));
	light_spot.setSpecularColor(ofColor(191, 191, 191));
	light_spot.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_spot.setSpotConcentration(2);
	light_spot.setSpotlightCutOff(30);
	light_spot.setSpotlight();

	ofLog() << "<reset>";
}

void Light::update() {
	ofPushMatrix();

	if (is_active_light_directional) {
		// transformer la lumi�re directionnelle avec la souris
		orientation_directional.makeRotate(int(ofGetElapsedTimeMillis() * 0.1f) % 360, 0, 1, 0);

		light_directional.setPosition(center_x, center_y + 60, camera_offset * 0.75f);
		light_directional.setOrientation(orientation_directional);
	}

	if (is_active_light_point) {
		// transformer la lumi�re ponctuelle avec la souris
		light_point.setPosition(ofGetMouseX(), ofGetMouseY(), camera_offset * 0.75f);
	}

	if (is_active_light_spot) {
		// transformer la lumi�re projecteur avec la souris
		oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude);

		orientation_spot.makeRotate(30.0f, ofVec3f(1, 0, 0), oscillation, ofVec3f(0, 1, 0), 0.0f, ofVec3f(0, 0, 1));

		light_spot.setOrientation(orientation_spot);

		light_spot.setPosition(center_x, center_y - 75.0f, camera_offset * 0.75f);
	}

	ofPopMatrix();
}

void Light::draw() {
	ofBackground(0);

	ofPushMatrix();

	// afficher un rep�re visuel pour les lumi�res
	if (is_active_light_point)
		light_point.draw();
	if (is_active_light_directional)
		light_directional.draw();
	if (is_active_light_spot)
		light_spot.draw();

	ofPopMatrix();

	// activer l'�clairage dynamique
	ofEnableLighting();

	// activer les lumi�res
	lighting_on();

	ofPushMatrix();

	// transformer l'origine de la sc�ne au milieu de la fen�tre d'affichage
	ofTranslate(center_x + offset_x, center_y, offset_z);

	// l�g�re rotation de la sc�ne
	ofRotateDeg(ofGetFrameNum() * 0.25f, 0, 1, 0);

	ofPushMatrix();

	// position
	ofTranslate(
		position_cube.x,
		position_cube.y,
		position_cube.z);

	// rotation locale
	ofRotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);

	// activer le mat�riau
	getCubeMaterial().begin();

	// dessiner un cube
	ofDrawBox(0, 0, 0, scale_cube);

	// d�sactiver le mat�riau
	getCubeMaterial().end();

	ofPopMatrix();

	ofPushMatrix();

	// position
	ofTranslate(
		position_sphere.x,
		position_sphere.y,
		position_sphere.z);

	// rotation locale
	ofRotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);

	// activer le mat�riau
	getSphereMaterial().begin();

	// dessiner une sph�re
	ofDrawSphere(0, 0, 0, scale_sphere);

	// d�sactiver le mat�riau
	getSphereMaterial().end();

	ofPopMatrix();

	// teapot (enlever pour les tests)
	/*
	ofPushMatrix();

	// position
	teapot.setPosition(
		position_teapot.x,
		position_teapot.y + 15,
		position_teapot.z);

	// rotation locale
	teapot.setRotation(0, ofGetFrameNum() * -1.0f, 0, 1, 0);

	// dimension
	teapot.setScale(
		scale_teapot,
		scale_teapot,
		scale_teapot);

	// d�sactiver le mat�riau par d�faut du mod�le
	teapot.disableMaterials();

	// activer le mat�riau
	material_teapot.begin();

	// dessiner un teapot
	teapot.draw(OF_MESH_FILL);

	// d�sactiver le mat�riau
	material_teapot.end();
	ofPopMatrix();
	*/

	ofPopMatrix();

	// d�sactiver les lumi�res
	lighting_off();

	// d�sactiver l'�clairage dynamique
	ofDisableLighting();
}

// d�sactivation des lumi�res dynamiques
void Light::lighting_on() {
	if (is_active_ligh_ambient)
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

// activation des lumi�res dynamiques
void Light::lighting_off() {
	ofSetGlobalAmbientColor(ofColor(0, 0, 0));
	light_directional.disable();
	light_point.disable();
	light_spot.disable();
}

// fonction d'oscillation
float Light::oscillate(float time, float frequency, float amplitude) {
	return sinf(time * 2.0f * PI / frequency) * amplitude;
}

