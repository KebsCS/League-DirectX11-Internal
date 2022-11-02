#pragma once

#include "Includes.h"
#include "GameObject.h"
#include "Render.h"

#include "CastSpell.h"

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
				float fGameTime = *reinterpret_cast<float*>(RVA(oGameTime));
				ImGui::Text("GameTime: %f", fGameTime);

				Vector3 mousePos = LeagueFuncs::GetMouseWorldPos();
				ImGui::Text("MousePos: %f  %f   %f", mousePos.x, mousePos.y, mousePos.z);

				GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));
				ImGui::Text(XorStr("AiManager: %#08x"), local->GetAiManager());

				static int fPoints = 16;
				ImGui::SliderInt("circle points", &fPoints, 1, 100);
				if (ImGui::Button("apply points"))
				{
					render.SetCirclePoints(fPoints);
				}

				LOGONCE("Menu 1");

				//for (auto b : local->GetBuffManager()->GetBuffList())
				//{
				//	ImGui::Text("%#04x %d %s : %0.1f", (DWORD)b, b->AltStackCount(), b->scriptBaseBuff->name, fabs(fGameTime - b->endTime));
				//	//	ImGui::Text("%#04x", (DWORD)b);
				//}

				ImGui::Text("%lf", local->GetRespawnTimeRemaining());
				ImGui::Checkbox("Stop console log", &Console::bStopLog);

				LOGONCE("Menu 2");


				static std::vector<Vector3>futurePath;
				static int iTestVal = 1;

				ImGui::InputInt("a", &iTestVal, 1, 2);

				//ImGui::Text("%lf", local->GetStatTotal((StatType)iTestVal));

				// tf why does this crash all of a sudden
				//render.HealthCircle(LeagueFuncs::WorldToScreen(local->position), local->health, local->maxhealth, 60);
				if (ImGui::Button("test"))
				{
					LeagueFuncs::DoEmote((EmoteType)iTestVal);
					LOGONCE("Menu 3");

					futurePath = local->GetAiManager()->BuildNavigationPath(mousePos, true);
					LOGONCE("Menu 4");

					//Vector3 testCast = local->position;
					//testCast.z += 500;
				//	CastSpell::QuickCastSpell(testCast, SpellSlotID::Q);

					//					POINT curMouse;
					//					bool getMouse = GetCursorPos(&curMouse);
					//					//if (!getMouse)
					//						//return false;
					//
					//#define SENDINPUTX(x) (x * 65536 / (::GetSystemMetrics(SM_CXSCREEN))+1)
					//#define SENDINPUTY(y) (y * 65536 / (::GetSystemMetrics(SM_CYSCREEN))+1)
					//					INPUT Input = { 0 };
					//					::ZeroMemory(&Input, sizeof(INPUT));
					//					Input.type = INPUT_MOUSE;
					//					Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
					//					Input.mi.dx = SENDINPUTX(20);
					//					Input.mi.dy = SENDINPUTY(20);
					//					::SendInput(1, &Input, sizeof(INPUT));
				}

				LOGONCE("menu3");

				ImGui::Text("%x", HashFunctions::HashStringELF("Aatrox"));

				LOGONCE("menu3.5");

				ImGui::Text("ad stattotal %lf", local->GetStatTotal(StatType::AttackDamage));

				LOGONCE("menu4");

				static char buff[100];
				ImGui::InputText("xd", buff, 100);
				std::string tmp = buff;
				ImGui::Text("%x", HashFunctions::HashStringCaseInsensitiveFNV1a(tmp.c_str()));

				LOGONCE("menu5");

				render.Path3D(futurePath, ImColor(0.f, 1.f, 1.f));

				LOGONCE("menu6");

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();

		ImGui::End();
	}

	/*static void Tab1();
	static void Tab2();*/
};
