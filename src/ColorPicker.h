#pragma once

#include "ofxImGui.h"

class ColorPicker
{
	public:
		void setup();
		void draw();

		bool showColorPicker;
		float selectedColor[4];

	private:
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

		/**
		* \brief Transforme un rgb en hsb (Hue, Saturation, Brightness)
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
		* \param float h(0, 360)
		* \param float s(0, 1)
		* \param float b(0, 1)
		* 
		* \return float r(0, 1)
		* \return float g(0, 1)
		* \return float b(0, 1)
		*/
		RGB HSBToRGB(float h, float s, float b);

		enum class ColorPickerMode { RGB, HSB };

		ColorPickerMode colorPickerMode;
};