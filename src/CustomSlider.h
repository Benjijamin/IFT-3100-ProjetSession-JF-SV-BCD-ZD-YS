#pragma once

#include "ofxImGui.h"

class CustomSlider
{
public:
	static void GradientSlider(const char* label, float* value, float valueMin, float valueMax, ImU32 colorStart, ImU32 colorEnd);
	static void HueSlider(const char* label, float* value, float valueMin, float valueMax);
private:
};