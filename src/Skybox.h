#pragma once

#include "ofMain.h"
#include "Primitives.h"
#include "CameraManager.h"

class Skybox
{
public:
	void setup();
	void draw();

	void setCamera(ofCamera& camera);
private:
	std::shared_ptr<ofMesh> model;
	ofImage image;
	ofShader shader;

	ofCamera* camera;
};
