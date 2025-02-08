#include "ColorPicker.h"

void ColorPicker::setup() 
{
	selectedColor[0] = 1.0f;
	selectedColor[1] = 1.0f;
	selectedColor[2] = 1.0f;
	selectedColor[3] = 1.0f;
}

void ColorPicker::draw() 
{
	if (showColorPicker) 
	{
		ImGui::Begin("ColorPicker", &showColorPicker, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("RGB")) colorPickerMode = ColorPickerMode::RGB;
		ImGui::SameLine();
		if (ImGui::Button("HSB")) colorPickerMode = ColorPickerMode::HSB;

		switch (colorPickerMode) 
		{
		default:
			break;
		case ColorPickerMode::RGB:
			ImGui::ColorPicker4("RGB", selectedColor, ImGuiColorEditFlags_DisplayRGB);
			break;
		case ColorPickerMode::HSB:
			ImGui::ColorPicker4("HSB", selectedColor, ImGuiColorEditFlags_DisplayHSV);
			break;
		}

		ImGui::End();
	}
}