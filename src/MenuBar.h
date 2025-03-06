#pragma once

#include "ofxImGui.h"
#include "DynamicCursor.h"  // Inclure la classe DynamicCursor


class MenuBar 
{
	public:
		void drawGui();
		void MenuBar::setup();
		bool isMouseOverMenuBar();  // Détecter si la souris est sur la barre de menus

private:
	DynamicCursor dynamicCursor;  // Instance de DynamicCursor
};