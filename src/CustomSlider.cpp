#include "CustomSlider.h"
#include <imgui_internal.h>

void CustomSlider::HueSlider(const char* label, float* value, float valueMin, float valueMax)
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
	ImU32 red = ImGui::GetColorU32(ImVec4(255, 0, 0, 1));
	ImU32 yellow = ImGui::GetColorU32(ImVec4(255, 255, 0, 1));
	ImU32 green = ImGui::GetColorU32(ImVec4(0, 255, 0, 1));
	ImU32 cyan = ImGui::GetColorU32(ImVec4(0, 255, 255, 1));
	ImU32 blue = ImGui::GetColorU32(ImVec4(0, 0, 255, 1));
	ImU32 magenta = ImGui::GetColorU32(ImVec4(255, 0, 255, 1));

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
	char cStringValue[50];
	sprintf(cStringValue, "%.3f", *value);
	ImVec2 textSize = ImGui::CalcTextSize(cStringValue);
	ImVec2 textPos = ImVec2(p.x + (width - textSize.x) * 0.5f, p.y + (height - textSize.y) * 0.5f);
	drawList->AddText(textPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), cStringValue);

	ImGui::SameLine();
	ImGui::Text(label);

	ImGui::PopID();
}

void CustomSlider::GradientSlider(const char* label, float* value, float valueMin, float valueMax, ImU32 colorStart, ImU32 colorEnd)
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
	drawList->AddRectFilled(ImVec2(grabX + margin, p.y + grabWidth), ImVec2(grabX + grabWidth, p.y + margin), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
	drawList->AddRectFilled(ImVec2(grabX + margin * 1.5f, p.y + grabWidth - margin * 0.5f), ImVec2(grabX + grabWidth - margin * 0.5f, p.y + margin * 1.5f), ImGui::GetColorU32(isActive ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab));

	//Draw value
	char cStringValue[50];
	sprintf(cStringValue, "%.3f", *value);
	ImVec2 textSize = ImGui::CalcTextSize(cStringValue);
	ImVec2 textPos = ImVec2(p.x + (width - textSize.x) * 0.5f, p.y + (height - textSize.y) * 0.5f);
	drawList->AddText(textPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), cStringValue);

	ImGui::SameLine();
	ImGui::Text(label);

	ImGui::PopID();
}
