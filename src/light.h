/**
 * \file light.h
 * \brief Implémentation des quatre types de lumière directionnelle
 * \author Yohan
 * \version 0.1
 * \date Mars 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */


#pragma once
#include "ofMain.h"

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
	void setup(LightType type);
	void update();
	void draw();

	void setPosition(const ofVec3f& position);
	void setDirection(const ofVec3f& direction);

	float oscillation;
	float oscillation_frequency;
	float oscillation_amplitude;
	float oscillate(float time, float frequency, float amplitude);

	ofVec3f getGlobalPosition() const;

	LightType getType() const { return type; }
	ofLight& getLight() { return light; }

private:
	LightType type;
	ofLight light;
};