#include "ColorPicker.h"
#include "CustomSlider.h"

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

			CustomSlider::HueSlider("Hue", &hueSlider, 0.0f, 360.0f);
			CustomSlider::GradientSlider("Saturation", &saturationSlider, 0.0f, 1.0f, saturationColorStart, saturationColorEnd);
			CustomSlider::GradientSlider("Brightness", &brightnessSlider, 0.0f, 1.0f, brightnessColorStart, brightnessColorEnd);
			CustomSlider::GradientSlider("Alpha", &selectedColor[3], 0.0f, 1.0f, alphaColorStart, alphaColorEnd);

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