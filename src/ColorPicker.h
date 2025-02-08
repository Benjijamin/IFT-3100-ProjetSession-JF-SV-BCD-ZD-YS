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
		enum class ColorPickerMode { RGB, HSB };

		ColorPickerMode colorPickerMode;
};