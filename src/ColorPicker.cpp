#include "ColorPicker.h"

void ColorPicker::setup() {
    color = ofFloatColor::white;
    currentItem = 0;
}

void ColorPicker::draw() {
    const char* items[] = { "RGB", "HSB" };

    ImGui::Begin("ColorPicker", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Combo("Mode", &currentItem, items, IM_ARRAYSIZE(items));

    ImGuiColorEditFlags flags = (currentItem == 0) ? ImGuiColorEditFlags_DisplayRGB : ImGuiColorEditFlags_DisplayHSV;
    float col[4] = { color.r, color.g, color.b, color.a };
    ImGui::ColorPicker4(items[currentItem], col, flags);
    color = ofFloatColor(col[0], col[1], col[2], col[3]);

    ImGui::End();
}

void ColorPicker::reset() {
    color = ofFloatColor::white;
    currentItem = 0;
}

ofFloatColor ColorPicker::getColor() const {
    return color;
}
