#pragma once

#include "ofMain.h"
#include "Primitives.h"
#include "CameraManager.h"

class Skybox
{
public:
	void setup();
	void draw();
	void drawGui();

	void setCamera(ofCamera& camera);

private:
	void drawSkybox() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	std::shared_ptr<ofMesh> model;
	ofImage image;
	ofShader shaderDefault;
	ofShader shaderCustom;

	ofCamera* camera;

	bool isCustom;
	glm::vec4 skyColor;
	glm::vec4 horizonColor;
	glm::vec4 bottomColor;
	float skyPower;
	float horizonPower;
	float bottomPower;
	bool enableStars;
};
