#include "MenuBar.h"

void MenuBar::drawGui() 
{
	if (ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("File")) 
		{
			if (ImGui::MenuItem("Settings")) 
			{
				//Ouvrir settings
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}