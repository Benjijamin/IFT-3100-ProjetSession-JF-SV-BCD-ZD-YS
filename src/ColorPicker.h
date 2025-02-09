#pragma once

#include "ofxImGui.h"

class ColorPicker
{
	public:
		void setup();
		void draw();

		struct HSB
		{
			float h;
			float s;
			float b;
		};

		struct RGB
		{
			float r;
			float g;
			float b;
		};

		bool showColorPicker;
		float selectedColor[4];
		HSB hsb;
		RGB rgb;
private:
		enum class ColorPickerMode { RGB, HSB };

		ColorPickerMode colorPickerMode;

		float hueSlider;
		float saturationSlider;
		float brightnessSlider;

		void GradientSlider(const char* label, float* value, float valueMin, float valueMax, ImU32 colorStart, ImU32 colorEnd);
		void HueSlider(const char* label, float* value, float valueMin, float valueMax);
		ImU32 red = ImGui::GetColorU32(ImVec4(255, 0, 0, 1));
		ImU32 yellow = ImGui::GetColorU32(ImVec4(255, 255, 0, 1));
		ImU32 green = ImGui::GetColorU32(ImVec4(0, 255, 0, 1));
		ImU32 cyan = ImGui::GetColorU32(ImVec4(0, 255, 255, 1));
		ImU32 blue = ImGui::GetColorU32(ImVec4(0, 0, 255, 1));
		ImU32 magenta = ImGui::GetColorU32(ImVec4(255, 0, 255, 1));

		/**
		* \brief Transforme un rgb en hsb (Hue, Saturation, Brightness)
		* Possiblement extraire en methode Utils si besoin ailleurs
		*
		* \param float r(0, 1)
		* \param float g(0, 1)
		* \param float b(0, 1)
		*
		* \return float hue(0, 360)
		* \return float saturation(0, 1)
		* \return float brightness(0, 1)
		*/
		HSB RGBToHSB(float r, float g, float b);

		/**
		* \brief Transforme un hsb (Hue, Saturation, Brightness) en rgb
		* Possiblement extraire en methode Utils si besoin ailleurs
		*
		* \param float h(0, 360)
		* \param float s(0, 1)
		* \param float b(0, 1)
		*
		* \return float r(0, 1)
		* \return float g(0, 1)
		* \return float b(0, 1)
		*/
		RGB HSBToRGB(float h, float s, float b);
};