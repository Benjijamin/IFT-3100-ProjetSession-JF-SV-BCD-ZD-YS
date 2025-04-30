#include "Skybox.h"

void Skybox::setup() 
{
	model = Primitives::getSkyboxPrimitive(1);
	image.load("skybox_default.png");
	shaderDefault.load("skybox_330_vs.glsl", "skybox_330_fs.glsl");
	shaderCustom.load("skybox_custom_330_vs.glsl", "skybox_custom_330_fs.glsl");

	skyColor = glm::vec4(0.2, 0.4, 0.4, 1);
	horizonColor = glm::vec4(0.2, 0.3, 0.3, 1);
	bottomColor = glm::vec4(0, 0.1, 0.1, 1);
}

void Skybox::draw() 
{
	if (!isCustom) {
		shaderDefault.begin();

		shaderDefault.setUniformMatrix4f("viewMatrix", getViewMatrix());
		shaderDefault.setUniformMatrix4f("projectionMatrix", getProjectionMatrix());
		shaderDefault.setUniformTexture("skyboxTexture", image.getTextureReference(), 1);

		drawSkybox();

		shaderDefault.end();
	}
	else 
	{
		shaderCustom.begin();

		shaderDefault.setUniformMatrix4f("viewMatrix", getViewMatrix());
		shaderDefault.setUniformMatrix4f("projectionMatrix", getProjectionMatrix());
		shaderCustom.setUniform4f("skyColor", skyColor);
		shaderCustom.setUniform4f("horizonColor", horizonColor);
		shaderCustom.setUniform4f("bottomColor", bottomColor);
		shaderCustom.setUniform1i("enableStars", enableStars);

		drawSkybox();

		shaderCustom.end();
	}
}

void Skybox::drawGui() 
{
	ImGui::Begin("Skybox", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Custom Skybox", &isCustom);
	if (isCustom) 
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Text("Sky Color");
		if (ImGui::ColorButton("Sky_Color", skyColor)) 
		{
			ImGui::OpenPopup("SkyColorPopup");
		}

		if (ImGui::BeginPopup("SkyColorPopup")) 
		{
			ImGui::ColorPicker4("##SkyColor", (float*)&skyColor);
			ImGui::EndPopup();
		}

		ImGui::Text("Horizon Color");
		if (ImGui::ColorButton("Horizon_Color", horizonColor))
		{
			ImGui::OpenPopup("HorizonColorPopup");
		}

		if (ImGui::BeginPopup("HorizonColorPopup"))
		{
			ImGui::ColorPicker4("##HorizonColor", (float*)&horizonColor);
			ImGui::EndPopup();
		}

		ImGui::Text("Bottom Color");
		if (ImGui::ColorButton("Bottom_Color", bottomColor))
		{
			ImGui::OpenPopup("BottomColorPopup");
		}

		if (ImGui::BeginPopup("BottomColorPopup"))
		{
			ImGui::ColorPicker4("##BottomColor", (float*)&bottomColor);
			ImGui::EndPopup();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Checkbox("Enable Stars", &enableStars);
	}

	ImGui::End();
}

void Skybox::drawSkybox() const
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	model->drawFaces();

	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
}

glm::mat4 Skybox::getViewMatrix() const
{
	glm::mat4 view = camera->getModelViewMatrix();

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[0][3] = 0.0f;
	view[1][3] = 0.0f;
	view[2][3] = 0.0f;

	return view;
}

glm::mat4 Skybox::getProjectionMatrix() const
{
	return camera->getProjectionMatrix();
}

void Skybox::setCamera(ofCamera& cam) 
{
	camera = &cam;
}