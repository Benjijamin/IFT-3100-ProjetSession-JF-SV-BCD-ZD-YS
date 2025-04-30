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

	glm::mat4 view = camera->getModelViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[0][3] = 0.0f;
	view[1][3] = 0.0f;
	view[2][3] = 0.0f;

	shader.setUniformMatrix4f("viewMatrix", view);
	shader.setUniformMatrix4f("projectionMatrix", projection);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	model->drawFaces();

	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
	
	shader.end();
}

void Skybox::setCamera(ofCamera& cam) 
{
	camera = &cam;
}