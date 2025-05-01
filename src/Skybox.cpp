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
	skyPower = 1.0f;
	horizonPower = 1.0f;
	bottomPower = 1.0f;
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
		shaderCustom.setUniform1f("skyPower", skyPower);
		shaderCustom.setUniform1f("horizonPower", horizonPower);
		shaderCustom.setUniform1f("bottomPower", bottomPower);

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
		ImGui::SliderFloat("Sky Power", &skyPower, 1.0f, 5.0f);

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
		ImGui::SliderFloat("Horizon Power", &horizonPower, 1.0f, 5.0f);

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
		ImGui::SliderFloat("Bottom Power", &bottomPower, 1.0f, 5.0f);

		if (ImGui::BeginPopup("BottomColorPopup"))
		{
			ImGui::ColorPicker4("##BottomColor", (float*)&bottomColor);
			ImGui::EndPopup();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Checkbox("Enable Stars", &enableStars);
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (ImGui::Button("Default")) 
	{
		image.load("skybox_default.png");
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (ImGui::Button("Load Cubemap")) 
	{
		ofFileDialogResult result = ofSystemLoadDialog("Select an image");
		if (result.bSuccess)
		{
			image.load(result.getPath());
		}
	}

	ImGui::End();
}

void Skybox::drawSkybox() const
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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

	//J'ai aucune pourquoi le fbo de post-processing flip uniquement la skybox, mais voici le duct-tape
	//Il faut aussi inverser l'ordre des vertex pour le culling
	glm::mat4 flipY = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));

	return flipY * view;
}

glm::mat4 Skybox::getProjectionMatrix() const
{
	return camera->getProjectionMatrix();
}

void Skybox::setCamera(ofCamera& cam) 
{
	camera = &cam;
}