#pragma once

#include "ofMain.h"
#include "Primitives.h"

class Skybox
{
public:
	void setup();
	void draw();
private:
	std::shared_ptr<ofMesh> model;
	ofImage image;
	ofShader shader;
};
