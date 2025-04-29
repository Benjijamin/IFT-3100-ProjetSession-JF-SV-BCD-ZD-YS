#include "Skybox.h"

void Skybox::setup() 
{
	model = Primitives::getSkyboxPrimitive(1000);
	image.load("skybox_default.png");
	shader.load("skybox_330_vs.glsl", "skybox_330_fs.glsl");
}

void Skybox::draw() 
{
	shader.begin();
	shader.setUniformTexture("skyboxTexture", image.getTextureReference(), 1);

	model->drawFaces();
	shader.end();
}

