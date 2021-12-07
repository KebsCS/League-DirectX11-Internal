#pragma once

#include "Includes.h"
#include "GameObject.h"
#include "Render.h"

class Menu
{
public:

	static void Render()
	{
		ImGui::SetNextWindowPos(ImVec2(904, 349), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(419, 421), ImGuiCond_FirstUseEver);;
		ImGui::Begin(XorStr("a"));// , ImGuiWindowFlags_AlwaysAutoResize);
		//if (ImGui::BeginPopupContextItem())
		//{
		//	//if (ImGui::MenuItem(XorStr("Open Console")))
		//	//	M.bConsoleOpen = true;
		//	//if (ImGui::MenuItem(XorStr("Exit")))
		//	//	M.bExitBot = true;
		//	//ImGui::EndPopup();
		//}

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		//ImGuiTreeNodeFlags collapsing_header_flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::BeginTabBar(XorStr("Tabs"), tab_bar_flags))
		{
			if (ImGui::BeginTabItem(XorStr("A")))
			{
				ImGui::Text("AiManager: %#08x", (*reinterpret_cast<GameObject**>(RVA(oLocalPlayer)))->GetAiManager());

				static int fPoints = 16;
				ImGui::SliderInt("circle points", &fPoints, 1, 100);
				if (ImGui::Button("apply points"))
				{
					render.SetCirclePoints(fPoints);
				}

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();

		ImGui::End();
	}

	/*static void Tab1();
	static void Tab2();*/
};
