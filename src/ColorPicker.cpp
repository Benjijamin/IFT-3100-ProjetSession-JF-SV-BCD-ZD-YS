#include "ColorPicker.h"
#include <imgui_internal.h>

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

		if (ImGui::Button("RGB")) 
			colorPickerMode = ColorPickerMode::RGB;
		ImGui::SameLine();
		if (ImGui::Button("HSB")) 
		{
			colorPickerMode = ColorPickerMode::HSB;
			HSB hsb = RGBToHSB(selectedColor[0], selectedColor[1], selectedColor[2]);
			hueSlider = hsb.h;
			saturationSlider = hsb.s;
			brightnessSlider = hsb.b;
		} 

		switch (colorPickerMode) 
		{
		default:
			break;
		case ColorPickerMode::RGB:
			ImGui::ColorPicker4("RGB", selectedColor, ImGuiColorEditFlags_DisplayRGB);
			break;
		case ColorPickerMode::HSB:
			float prevHue = hueSlider, prevSaturation = saturationSlider, prevBrightness = brightnessSlider;

			HSB saturationHSBNone, saturationHSBFull = hsb;
			saturationHSBNone.s = 0.0f;
			saturationHSBFull.s = 1.0f;
			RGB saturationRGBStart = HSBToRGB(saturationHSBNone.h, saturationHSBNone.s, saturationHSBNone.b);
			RGB saturationRGBEnd = HSBToRGB(saturationHSBFull.h, saturationHSBFull.s, saturationHSBFull.b);

			ImU32 saturationColorStart = ImGui::GetColorU32(ImVec4(saturationRGBStart.r, saturationRGBStart.g, saturationRGBStart.b, 1.0f));
			ImU32 saturationColorEnd = ImGui::GetColorU32(ImVec4(saturationRGBEnd.r, saturationRGBEnd.g, saturationRGBEnd.b, 1.0f));

			ImU32 brightnessColorStart = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImU32 brightnessColorEnd = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImU32 alphaColorStart = ImGui::GetColorU32(ImVec4(selectedColor[0], selectedColor[1], selectedColor[2], 0.0f));
			ImU32 alphaColorEnd = ImGui::GetColorU32(ImVec4(selectedColor[0], selectedColor[1], selectedColor[2], 1.0f));

			HueSlider("Hue", &hueSlider, 0.0f, 360.0f);
			GradientSlider("Saturation", &saturationSlider, 0.0f, 1.0f, saturationColorStart, saturationColorEnd);
			GradientSlider("Brightness", &brightnessSlider, 0.0f, 1.0f, brightnessColorStart, brightnessColorEnd);
			GradientSlider("Alpha", &selectedColor[3], 0.0f, 1.0f, alphaColorStart, alphaColorEnd);

			if (prevHue != hueSlider || prevSaturation != saturationSlider || prevBrightness != brightnessSlider) 
			{
				rgb = HSBToRGB(hueSlider, saturationSlider, brightnessSlider);
				hsb = { hueSlider, saturationSlider, brightnessSlider };
				selectedColor[0] = rgb.r, selectedColor[1] = rgb.g, selectedColor[2] = rgb.b;
			}

			break;
		}

		ImGui::End();
	}
}

void ColorPicker::HueSlider(const char* label, float* value, float valueMin, float valueMax) 
{
	ImGui::PushID(label);
	ImVec2 p = ImGui::GetCursorScreenPos();
	float width = ImGui::CalcItemWidth();
	float height = ImGui::GetFrameHeight();
	float margin = 1.5f;
	float grabWidth = height - margin;
	ImRect grabBoundingBox = ImRect(ImVec2(p.x + height * 0.5f, p.y), ImVec2(p.x + width - height * 0.5f, p.y + height));

	ImGui::InvisibleButton(label, ImVec2(width, height));
	bool isActive = ImGui::IsItemActive();
	bool clicked = ImGui::IsItemClicked();

	if (isActive)
	{
		float mousePos = ImGui::GetMousePos().x - grabBoundingBox.Min.x;
		*value = valueMin + (valueMax - valueMin) * (mousePos / grabBoundingBox.GetWidth());
		*value = ImClamp(*value, valueMin, valueMax);
	}

	//Draw background
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float sixth = width / 6.0f;

	drawList->AddRectFilledMultiColor(ImVec2(p.x, p.y), ImVec2(p.x + sixth, p.y + height), red, yellow, yellow, red);
	drawList->AddRectFilledMultiColor(ImVec2(p.x + sixth, p.y), ImVec2(p.x + sixth * 2, p.y + height), yellow, green, green, yellow);
	drawList->AddRectFilledMultiColor(ImVec2(p.x + sixth * 2, p.y), ImVec2(p.x + sixth * 3, p.y + height), green, cyan, cyan, green);
	drawList->AddRectFilledMultiColor(ImVec2(p.x + sixth * 3, p.y), ImVec2(p.x + sixth * 4, p.y + height), cyan, blue, blue, cyan);
	drawList->AddRectFilledMultiColor(ImVec2(p.x + sixth * 4, p.y), ImVec2(p.x + sixth * 5, p.y + height), blue, magenta, magenta, blue);
	drawList->AddRectFilledMultiColor(ImVec2(p.x + sixth * 5, p.y), ImVec2(p.x + sixth * 6, p.y + height), magenta, red, red, magenta);

	//Draw grab
	float grabPos = (*value - valueMin) / (valueMax - valueMin) * grabBoundingBox.GetWidth();
	float grabX = p.x + grabPos;
	drawList->AddRectFilled(ImVec2(grabX + margin, p.y + grabWidth), ImVec2(grabX + grabWidth, p.y + margin), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
	drawList->AddRectFilled(ImVec2(grabX + margin * 1.5f, p.y + grabWidth - margin * 0.5f), ImVec2(grabX + grabWidth - margin * 0.5f, p.y + margin * 1.5f), ImGui::GetColorU32(isActive ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab));

	//Draw value
	ImVec2 textSize = ImGui::CalcTextSize(label);
	ImVec2 textPos = ImVec2(p.x + (width - textSize.x) * 0.5f, p.y + (height - textSize.y) * 0.5f);
	char cStringValue[50];
	sprintf(cStringValue, "%.3f", *value);
	drawList->AddText(textPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), cStringValue);

	ImGui::SameLine();
	ImGui::Text(label);

	ImGui::PopID();
}

void ColorPicker::GradientSlider(const char* label, float* value, float valueMin, float valueMax, ImU32 colorStart, ImU32 colorEnd) 
{
	ImGui::PushID(label);
	ImVec2 p = ImGui::GetCursorScreenPos();
	float width = ImGui::CalcItemWidth();
	float height = ImGui::GetFrameHeight();
	float margin = 1.5f;
	float grabWidth = height - margin;
	ImRect grabBoundingBox = ImRect(ImVec2(p.x + height * 0.5f, p.y), ImVec2(p.x + width - height * 0.5f, p.y + height));

	ImGui::InvisibleButton(label, ImVec2(width, height));
	bool isActive = ImGui::IsItemActive();
	bool clicked = ImGui::IsItemClicked();

	if (isActive) 
	{
		float mousePos = ImGui::GetMousePos().x - grabBoundingBox.Min.x;
		*value = valueMin + (valueMax - valueMin) * (mousePos / grabBoundingBox.GetWidth());
		*value = ImClamp(*value, valueMin, valueMax);
	}

	//Draw background
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilledMultiColor(p, ImVec2(p.x + width, p.y + height), colorStart, colorEnd, colorEnd, colorStart);

	//Draw grab
	float grabPos = (*value - valueMin) / (valueMax - valueMin) * grabBoundingBox.GetWidth();
	float grabX = p.x + grabPos;
	drawList->AddRectFilled(ImVec2(grabX + margin, p.y + grabWidth), ImVec2(grabX + grabWidth, p.y + margin), ImGui::GetColorU32(ImVec4(0.0f,0.0f,0.0f,1.0f)));
	drawList->AddRectFilled(ImVec2(grabX + margin * 1.5f, p.y + grabWidth - margin * 0.5f), ImVec2(grabX + grabWidth - margin * 0.5f, p.y + margin * 1.5f), ImGui::GetColorU32(isActive ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab));

	//Draw value
	ImVec2 textSize = ImGui::CalcTextSize(label);
	ImVec2 textPos = ImVec2(p.x + (width - textSize.x) * 0.5f, p.y + (height - textSize.y) * 0.5f);
	char cStringValue[50];
	sprintf(cStringValue, "%.3f", *value);
	drawList->AddText(textPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), cStringValue);

	ImGui::SameLine();
	ImGui::Text(label);

	ImGui::PopID();
}

ColorPicker::HSB ColorPicker::RGBToHSB(float r, float g, float b) 
{
	float max = std::max({ r, g, b });
	float min = std::min({ r, g, b });
	float delta = max - min;

	HSB hsb;
	hsb.b = max;

	if (delta < 0.001 || max == 0) //grayscale 
	{
		hsb.h = 0;
		hsb.s = 0;
		return hsb;
	}

	hsb.s = delta / max;

	if (r == max) //cadrant rouge
	{
		hsb.h = (g - b) / delta;
	}
	else if (g == max) //cadrant vert
	{
		hsb.h = 2.0f + (b - r) / delta;
	}
	else //cadrant bleu
	{
		hsb.h = 4.0f + (r - g) / delta;
	}

	//conversion en degre
	hsb.h *= 60.0f;
	if (hsb.h < 0.0f) hsb.h += 360.0f;

	return hsb;
}

ColorPicker::RGB ColorPicker::HSBToRGB(float h, float s, float b) 
{
	RGB rgb;

	if (s == 0) //grayscale
	{
		rgb.r = b;
		rgb.g = b;
		rgb.b = b;
		return rgb;
	}

	float hue = h;
	if (hue == 360.0) hue = 0.0f;
	hue /= 60.0;

	int i = (int)hue; //cadrant
	float ff = hue - i; //fraction dans cadrant
	float p = b * (1.0f - s);
	float q = b * (1.0f - (s * ff));
	float t = b * (1.0f - (s * (1.0f - ff)));

	switch (i) 
	{
	case 0:
		rgb.r = b;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = b;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = b;
		rgb.b = t;
		break;
	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = b;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = b;
		break;
	case 5:
	default:
		rgb.r = b;
		rgb.g = p;
		rgb.b = q;
	}

	return rgb;
}