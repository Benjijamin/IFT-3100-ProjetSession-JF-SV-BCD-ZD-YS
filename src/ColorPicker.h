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
private:
		enum class ColorPickerMode { RGB, HSB };

		HSB hsb;
		RGB rgb;

		ColorPickerMode colorPickerMode;

		float hueSlider;
		float saturationSlider;
		float brightnessSlider;

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