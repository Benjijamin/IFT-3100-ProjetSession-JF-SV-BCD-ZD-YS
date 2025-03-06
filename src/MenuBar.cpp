#include "MenuBar.h"


void MenuBar::setup() {
	dynamicCursor.setup();  // Initialiser DynamicCursor
}
void MenuBar::drawGui() 
{
        // Démarrer la barre de menus
        if (ImGui::BeginMainMenuBar()) {
            // Onglet "File"
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Settings")) {
                    // Ouvrir settings
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

}

bool MenuBar::isMouseOverMenuBar() {
    // Utiliser ImGui pour détecter si la souris est sur la barre de menus
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}