#include "Hooks.h"
#include "Ntdefs.h"
#include "Render.h"
#include "./Fonts/Tahoma.h"
#include "./Fonts/CascadiaMono.h"
#include "Ntfuncs.h"
#include "LeagueClasses.h"
#include "LeagueFuncs.h"
#include "ObjectManager.h"
#include "Menu.h"
#include "GameObject.h"

#include "Keyboard.h"
#include "Mouse.h"

#include "FuncHook.h"
#include "VMTHook.h"
#include "VFuncHook.h"

#include "OnProcessSpellCast.h"
#include "LowLevelHooks.h"
#include "AntiInputHooks.h"

#include "IssueOrder.h"
#include "CastSpell.h"

#include "Prediction.h"

typedef HRESULT(WINAPI* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
D3D11PresentHook				phookD3D11Present = nullptr;
uintptr_t DiscordPresent = 0;
/*
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
D3D11DrawIndexedHook            phookD3D11DrawIndexed = nullptr;
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
D3D11CreateQueryHook			phookD3D11CreateQuery = nullptr;
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
D3D11PSSetShaderResourcesHook	phookD3D11PSSetShaderResources = nullptr;
typedef void(__stdcall* D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);
D3D11ClearRenderTargetViewHook  phookD3D11ClearRenderTargetViewHook = nullptr;
*/
typedef HRESULT(WINAPI* IDirectInputDeviceGetDeviceDataHook)(IDirectInputDevice8W* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
IDirectInputDeviceGetDeviceDataHook phookGetDeviceData = nullptr;

ID3D11Device* pDevice = nullptr;
IDXGISwapChain* pSwapchain = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
ID3D11Texture2D* pBackbuffer = nullptr;

IDirectInput8W* pDirectInput = nullptr;

LowLevelHooks lowLevelHooks;
AntiInputHooks antiInputHooks;

// move this?
FuncHook FHPresent;
FuncHook FHGetDeviceData;
//VFuncHook VFHGetDeviceDataHook;

FuncHook FHTestFunc;

void mouse_movement_detection(HRAWINPUT raw_input)
{
	UINT buffer_size = 48;
	LPBYTE buffer[48];
	GetRawInputData((HRAWINPUT)raw_input, RID_INPUT, buffer, &buffer_size, sizeof(RAWINPUTHEADER));
	RAWINPUT* raw = (RAWINPUT*)buffer;
	if (raw->header.dwType != RIM_TYPEMOUSE) {
		return;
	}
	const RAWMOUSE& mouse = raw->data.mouse;
	if ((mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE) {
		LOG("detected");
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD now = FGetTickCount();
	Keyboard::Update(now);
	Mouse::Update(now);

	// hotkeys
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_INSERT)
			Globals::bMenuOpen = !Globals::bMenuOpen;

#ifdef DEVELOPER
		//mouse_movement_detection((HRAWINPUT)lParam);

		if (wParam == VK_F5)
		{
			// hook GetProcessMitigationPolicy to always return 0 for dep so its ud
			/*DWORD flags = 0;
			BOOL pernament = 0;
			if (GetProcessDEPPolicy(GetCurrentProcess(), &flags, &pernament))
			{
				FSetProcessDEPPolicy();
				LOG("%d %d", flags, pernament);
			}*/
			//}
			//LI_FN(MessageBoxA).get()(0, "", "", 0);
		}
#endif
	}

	if (Globals::bMenuOpen)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;
	}

	return LI_FN(CallWindowProcW).get()(Globals::WndProc, hwnd, uMsg, wParam, lParam);
}

static HRESULT WINAPI Hooks::GetDeviceDataHook(IDirectInputDevice8W* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	// Call original function to get actual keypresses
#ifdef USEMINHOOK
	HRESULT ret = phookGetDeviceData(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);
#else
	// fix hooking method
	HRESULT ret = FHGetDeviceData.Call<HRESULT>(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);
	//HRESULT ret = phookGetDeviceData(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);

#endif

	if (ret == DI_OK)
	{
		for (DWORD i = 0; i < *pdwInOut; i++)
		{
			//if (bXerathAimbot)
			{
				//todo xerath stuff, move to class, less scuffed when not holding, or dont aim at all when tapping
				// and better detection if holding, instead of inputs ==0, maybe if has xerath buff holding q
				if (rgdod[i].dwOfs == DIK_Q)
				{
					LOG("dwOfs %d dwData %d  dwTimeStamp %d dwSequence %d", rgdod[i].dwOfs, rgdod[i].dwData, rgdod[i].dwTimeStamp, rgdod[i].dwSequence);

					// button released
					if (rgdod[i].dwData == 0 && (CastSpell::InputsSize() == 0))
					{
						GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));
						Vector3 mousePos = LeagueFuncs::GetMouseWorldPos();
						float max = 999999;
						GameObject* target;
						for (GameObject* enemy : ObjectManager::HeroList())
						{
							if (enemy != local)
							{
								if (mousePos.Distance(enemy->position) < max)
								{
									max = mousePos.Distance(enemy->position);
									target = enemy;
								}
							}
						}
						if (max < 500.f)
						{
							auto p = Prediction::Preidct33(local, target, 1600, 95, 0.5, 999999, 0);
							CastSpell::Cast(DIK_Q, LeagueFuncs::WorldToScreen(p));
							rgdod[i].dwOfs = 0;
						}
					}
				}
			}

			if (LOBYTE(rgdod[i].dwData) > 0)
			{
				//LOG("%d %d %d %d %d", rgdod[i].dwOfs, rgdod[i].dwData, rgdod[i].dwTimeStamp, rgdod[i].dwSequence, rgdod[i].uAppData);
				// block input when menu is open
				if (Globals::bMenuOpen)
				{
					rgdod[i].dwData = 0;
					rgdod[i].dwOfs = 0;
				}

				//switch (rgdod[i].dwOfs)
				{
					// blocks W key by changing key code to 0
				/*case DIK_W:
					rgdod[i].dwOfs = 0;
					break;*/
				}
			}
		}
	}

	return ret;
}

static std::vector<Vector3>testpos;

uintptr_t pTestFunc;
//void __cdecl TestFunc(unsigned int a1, int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, float a8, unsigned int a9)
////DWORD* __fastcall TestFunc(void* thisptr, void* edx, int a2)
//{
//	//std::stringstream ss;
//
//	LOG("%#04x", a1);
//
//	static auto fn = reinterpret_cast<void (__cdecl*)(unsigned,int,unsigned,unsigned,unsigned,unsigned,unsigned,float,unsigned)>(pTestFunc);
//	return fn(a1,a2,a3,a4,a5,a6,a7,a8,a9);
//}

//char __cdecl TestFunc(float* a1)
//{
//	static auto fn = reinterpret_cast<char(__cdecl*)(float*)>(pTestFunc);
//
//	LOG("%f %f %f", a1[0], a1[1], a1[2]);
//
//	//return FHTestFunc.Call<char>(a1);
//
//	return fn(a1);
//}

void TestFuncs()
{
	GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));
	LOG("Trying IsBrush");
	LOG("IsBrush expected 1: %d", LeagueFuncs::IsBrush(Vector3(7066, 52, 3142)));
	LOG("IsBrush expected 0: %d", LeagueFuncs::IsBrush(Vector3(7467, 52, 3120)));

	LOG("Trying IsNotWall");
	LOG("IsNotWall expected 1: %d", LeagueFuncs::IsNotWall(Vector3(7467, 52, 3120)));
	LOG("IsNotWall expected 0: %d", LeagueFuncs::IsNotWall(Vector3(6691, 49, 3570)));

	LOG("Trying StatTotal");
	LOG("StatTotal HP expected %f: %f", local->maxhealth, local->GetStatTotal(StatType::Health, StatInfo::TOTAL));

	LOG("AiManager");
	LOG("ai %x", local->GetAiManager());
}

// todo, IAT/EAT hook(if undetected),
//FuncHook FHGetCursorPos;
//static BOOL WINAPI GetCursorPosHook(LPPOINT lpPoint)
//{
//	auto ret = FHGetCursorPos.Call<BOOL>(lpPoint);
//
//	//POINT& p = *lpPoint;
//	if (HookedMouse.enabled)
//	{
//		LOG("before lock");
//		HookedMouse.mutex.lock();
//		*lpPoint = HookedMouse.pos;
//		HookedMouse.mutex.unlock();
//		HookedMouse.enabled = false;
//		LOG("after lock");
//	}
//
//	return ret;
//}

static HRESULT WINAPI Hooks::PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static std::once_flag isInitialized;

	std::call_once(isInitialized, [&]()
		{
			InitD3D(pSwapChain);
			InitImgui(pSwapChain);

			//lowLevelHooks.Init(); todo, they need thread with GetMessage and dont seem to work with internal
			antiInputHooks.Init();

			InitDInput();

			HookOnProcessSpellCast();

			//FHGetCursorPos = FuncHook((uintptr_t)GetCursorPos, (uintptr_t)GetCursorPosHook);
			//FHGetCursorPos.Hook();

#ifdef  DEVELOPER

			TestFuncs();

#endif //  DEVELOPER

			/*		DWORD func = RVA(0x9ABBB0);
						LOG("%#04x", func);
						FHTestFunc = FuncHook((uintptr_t)func, (uintptr_t)TestFunc);
						FHTestFunc.Hook();*/
						/*DWORD func = *reinterpret_cast<DWORD*>(RVA(0x9ABBB0));
						if (MH_CreateHook((DWORD_PTR*)func, TestFunc, reinterpret_cast<void**>(&pTestFunc)) != MH_OK) { return false; }
						if (MH_EnableHook((DWORD_PTR*)func) != MH_OK) { return false; }*/
		});

	// todo sometimes on unloading pContext was nullptr, maybe make a mutex of some sort
	if (pContext)
	{
		LOGONCE("Before OMSetRenderTargets");
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

		::ImGui_ImplDX11_NewFrame();
		::ImGui_ImplWin32_NewFrame();
		::ImGui::NewFrame();

		//if (Globals::bMenuOpen)
		{
			::ImGui::ShowDemoWindow();
			::ImGui::ShowMetricsWindow();

			LOGONCE("Before Viewmatrix update");

			// todo, make update per tick class
			Globals::viewMatrix = LeagueFuncs::GetViewMatrix();
			Globals::projectionMatrix = LeagueFuncs::GetProjectionMatrix();
			Matrix4x4::MultiplyMatrices(Globals::multipliedMatrix, Globals::viewMatrix.matrix, Globals::projectionMatrix.matrix);

			render.RenderScene();

			LOGONCE("After RenderScene");

			CastSpell::Update();

			//LOG("After render scene");

			// VISUALS GO HERE
			// -----

			//render.Box(20, 100, 100, 200, ImColor(1.f, 0.f, 0.f));

			//ImGuiIO& io = ImGui::GetIO();
			//ImVec2 mpos = io.MousePos;
			////render.Text("xddd", mpos.x, mpos.y, 20.f, ImColor(1.f, 0.f, 0.f), true, true);
			////Image aatroxe = imageManager.GetImageInfoByName("aatrox_square");
			////render.Image(mpos.x, mpos.y, aatroxe.width, aatroxe.height, aatroxe.pShaderResource, true);

			//render.ImageBordered(10, 10, 64, 64, imageManager.GetImageByName(XorStr("talon_w")), true);

			//render.CornerBox(300, 300, 400, 400, ImColor(0.f, 1.f, 0.f));

			//static float cd = 200.f;
			//if (cd < -200.f)
			//	cd = 200.f;

			//render.FancyIcon(150, 150, XorStr("talon"), cd / 200.f, cd / 200.f, cd / 200.f, 1, cd, XorStr("summoner_flash"), cd, XorStr("summoner_heal"), cd);
			//cd -= 1.f;

			for (auto& pos : testpos)
			{
				Vector2 pos2d = LeagueFuncs::WorldToScreen(pos);
				render.Circle(pos2d.x, pos2d.y, 32, 16, ImColor(1.f, 0.f, 0.f));
			}

			LOGONCE("Present 1");

			/*for (auto& poly : testpoly)
				render.Polygon(poly, ImColor(0.f, 0.f, 1.f));*/

				/*auto danger_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(testpoly));

				for (auto& poly : danger_polygons)
					render.Polygon(poly, ImColor(0.f, 0.f, 1.f));*/

			GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));


			LOGONCE("Present 2");
			/*{
				auto fwaypoint = local->GetAiManager()->GetFuturePoints();
				if (fwaypoint.size() >= 2)
				{
					Vector3 disp = fwaypoint[1] - fwaypoint[0];
					float distance = disp.Length();
					Vector3 velocity = disp * (local->GetStatTotal(StatType::MoveSpeed) / distance);
					Vector3 realvel = local->GetAiManager()->velocity;
					LOG("my %f %f %f", velocity.x, velocity.y, velocity.z);
					LOG("re %f %f %f", realvel.x, realvel.y, realvel.z);
				}
			}*/

			//LOG("spellstate q %d", local->GetSpellState(SpellSlotID::Q));

			//LeagueFuncs::FindVFunc(((DWORD)local + 0x21b8), 0x0);

			for (auto& h : ObjectManager::HeroList())
				render.Circle3D(h->GetAiManager()->serverPos, h->GetBoundingRadius(), ImColor(0.f, 1.f, 0.f));

			LOGONCE("Present 3");

			//LeagueFuncs::FindVFunc((DWORD)local, 0x1BC230); // shoulddrawheal
			//LeagueFuncs::FindVFunc((DWORD)local, 0x16A0E0); // highlih
			//LeagueFuncs::FindVFunc((DWORD)local, 0x146230); // resp

			ImColor mousecol;
			if (LeagueFuncs::IsBrush(LeagueFuncs::GetMouseWorldPos()))
				mousecol = ImColor(1.f, 0.f, 0.f);
			else
				mousecol = ImColor(0.f, 1.f, 0.f);

			render.Circle3D(LeagueFuncs::GetMouseWorldPos(), 65.f, mousecol);

			Vector3 mousePos = LeagueFuncs::GetMouseWorldPos();
			Vector2 mousePos2D = LeagueFuncs::WorldToScreen(mousePos);
			std::string col = std::to_string(LeagueFuncs::GetCollisionFlags(mousePos));
			render.Text(col, mousePos2D.x, mousePos2D.y + 30);

			static GameObject* enemy;

			DWORD now = FGetTickCount();
			static DWORD lastTick = now + 100000;
			//if ((now - lastTick > 1000) && LI_FN(GetAsyncKeyState).get()(VK_F5))
			{
				lastTick = now;
				POINT curMouse(50, 50);
				bool getMouse = LI_FN(GetCursorPos).get()(&curMouse);

				for (auto h : ObjectManager::HeroList())
					if (h != local)
						enemy = h;

				LOGONCE("Present 4");

				// xerath w
				//auto p = Prediction::Preidct33(local, enemy, 1500, 60, 0.75, 99999, 0);

				// ezreal q
				//auto p = Prediction::Preidct33(local, enemy, 1200, 60, 0.25, 2000, 0);

				//LOG("eee2");


				//render.Circle3D(p, 65, ImColor(0.f, 0.f, 1.f));
				//if (LI_FN(GetAsyncKeyState).get()(VK_F5) & 1)
				//{
				//	//Mouse::Move(LeagueFuncs::WorldToScreen(p));
				//	//Keyboard::KeyDown(DIK_Q);
				//	//IssueOrder::AttackMove(curMouse);
				//	CastSpell::Cast(DIK_Q, LeagueFuncs::WorldToScreen(p));
				//}

				/*	Vector3 predict = Prediction::Preidct33(local, enemy, 1500, 60, 0.6, 3300, 0);
					render.Circle3D(predict, 65,ImColor(0.f,0.f,1.f));
					if (LI_FN(GetAsyncKeyState).get()(VK_F5))
					{
						Mouse::MouseBack(LeagueFuncs::WorldToScreen(predict));
						Keyboard::KeyDown(DIK_W);
					}*/

				{
					//IssueOrder::AttackMove(curMouse);
					//Mouse::RightDown();
				}
			}
			if (enemy)
			{
				//render.Circle3D(Prediction::PosAfterTime(enemy, 0.75), 65.f);
				render.Circle3D(local->position, 1000);
			}

			LOGONCE("Present 5");


			//std::vector<Vector3>points = *reinterpret_cast<std::vector<Vector3>*>(local->GetAiManager() + 0x1E4);

			//Vector3 points = **reinterpret_cast<Vector3**>(local->GetAiManager() + 0x1E4);
			//DWORD arr = *reinterpret_cast<DWORD*>(local->GetAiManager() + 0x1E4);
			//Vector3 points = *reinterpret_cast<Vector3*>(arr);
			//render.Circle3D(points, 30.f, ImColor(1.f, 1.f, 1.f));

			//for (auto& minion : ObjectManager::MinionList())
			//{
			//	//if (minion->maxhealth == 3.f && minion->health > 0.f && minion->mana > 0.f)
			//	//	render.RealWardRange(minion->position, ImColor(1.f, 1.f, 1.f));

			//	if (minion && minion->health > 0.f && minion->GetBoundingRadius())
			//	{
			//		render.Circle3D(minion->position, minion->GetBoundingRadius(), ImColor(1.f, 0.f, 0.f));
			//		Vector2 objPos = LeagueFuncs::WorldToScreen(minion->position);
			//		render.Text(minion->AddressHex(), objPos.x, objPos.y);

			//		if (minion->IsWard())
			//		{
			//			render.Text("WARD", objPos.x, objPos.y);

			//		}
			//	}

			//
			//}

			LOGONCE("Present 6");


			for (auto& obj : ObjectManager::ObjectList())
			{
				LOGONCE("Present 7");
				if (obj)
				{
					if (obj->IsAlive())
					{
						LOGONCE("Present 8 after alive");
						//LeagueFuncs::FindVFunc((DWORD)obj, 0x13FA70);
						Vector2 objPos = LeagueFuncs::WorldToScreen(obj->position);
						//std::string nn = obj->name.str() + std::to_string((DWORD)obj);
						render.Text(obj->AddressHex(), objPos.x, objPos.y);

						//LOG("%s", obj->name);
					}
				}
			}

			LOGONCE("Present 9");

			
			//render.Circle3D(Prediction::PosAfterTime(local, 1, 0), 65.f); // broken todo fix


			LOGONCE("Present 10");

			/*for (auto& obj : ObjectManager::ObjectMap())
			{
				if (obj.first && obj.second)
				{
					Vector2 objPos = LeagueFuncs::WorldToScreen(obj.second->position);

					render.Text(obj.second->name, objPos.x, objPos.y, 13.f, ImColor(0.f, 1.f, 0.f));
				}
			}*/

			//for (auto& obj : ObjectManager::SomeList3())
			//{
			//	if (obj.first && obj.second)
			//	{
			//		Vector2 objPos = LeagueFuncs::WorldToScreen(obj.second->position);

			//		LOG("%s", obj.second->name);
			//		//render.Text(obj.second->name, objPos.x, objPos.y, 13.f, ImColor(0.f, 1.f, 0.f));
			//	}
			//}
			{
				/*std::vector<Vector3>pathList = local->GetAiManager()->GetWaypoints();
				for (auto& point : pathList)
				{
					render.Circle3D(point, 15.f, ImColor(1.f, 1.f, 1.f));
				}*/
			/*	AiManager* ai = local->GetAiManager();
				render.Path3D(ai->GetWaypoints());
				render.Path3D(ai->GetFuturePoints(), ImColor(0.f, 0.f, 1.f));
				for (Vector3& wpoint : ai->GetWaypoints())
				{
					render.Circle3D(wpoint, 10);
				}
				Vector2 pathEnd = LeagueFuncs::WorldToScreen(ai->destination);
				render.Text(std::to_string(ai->GetPathLength()), pathEnd.x, pathEnd.y - 25.f);
				render.Text(std::to_string(ai->GetPathLength() / ai->movementSpeed), pathEnd.x, pathEnd.y - 45.f);
				render.Text(std::to_string(ai->GetWaypoints().size()), pathEnd.x, pathEnd.y - 65.f);*/
			}

			//render.Circle3D(local->position, local->GetBoundingRadius() - 5.f, ImColor(1.f, 0.f, 0.f));

			LOGONCE("Present 11");

			if (Globals::bMenuOpen)
			{
				Menu::Render();
			}
		}
		//else
		//{
		//	testpos.clear();
		//	//testpoly.clear();
		//}
		// -----

		::ImGui::EndFrame();
		::ImGui::Render();

		::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		LOGONCE("Present finished");
	}

#ifdef DISCORD
	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
#else

#ifdef USEMINHOOK
	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
#else
	return FHPresent.Call<HRESULT>(pSwapChain, SyncInterval, Flags);
#endif
#endif
}

inline uintptr_t FindPattern(void* module, const char* szSignature) {
	static auto patternToByte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + crt::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	const auto dosHeader = (PIMAGE_DOS_HEADER)module;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(szSignature);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
	}
	return 0;
}

bool Hooks::Init()
{
#ifdef DISCORD
	HANDLE discordHandle = 0;
	auto skDiscord = skCrypt("DiscordHook.dll");
	while (!discordHandle)
		discordHandle = GetModuleBase(skDiscord);
	skDiscord.clear();

	auto skPattern = skCrypt("FF 15 ? ? ? ? 89 C6 8D 4D EC E8 ? ? ? ? 8B 4D F0 31 E9 E8 ? ? ? ? 89 F0 83 C4 08 5E 5F 5B 5D C2 0C 00");
	DiscordPresent = FindPattern(discordHandle, skPattern) + 0x2;
	skPattern.clear();

	LOGONCE("Discord address %x", DiscordPresent);

	phookD3D11Present = **reinterpret_cast<D3D11PresentHook**>(DiscordPresent);
	**reinterpret_cast<D3D11PresentHook**>(DiscordPresent) = reinterpret_cast<D3D11PresentHook>(PresentHook);

#else
	D3D_FEATURE_LEVEL FeatureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0/*, D3D_FEATURE_LEVEL_10_1 */ };
	D3D_FEATURE_LEVEL FeatureLevelsSupported;

	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));

		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		sd.OutputWindow = Globals::Hwnd;
		sd.Windowed = true;// ((GetWindowLongPtr(Globals::Hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}

	// 	std::string szd3d11 = XorStr("d3d11.dll");
	// std::string szD3D11CreateDeviceAndSwapChain = XorStr("D3D11CreateDeviceAndSwapChain");
	//HRESULT hr = ModuleCall<HRESULT>(szd3d11.c_str(), szD3D11CreateDeviceAndSwapChain.c_str(),
	//	nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, FeatureLevelsRequested,
	//	sizeof(FeatureLevelsRequested) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
	//	&sd, &pSwapchain, &pDevice, &FeatureLevelsSupported, &pContext);

	HRESULT hr = LI_FN(D3D11CreateDeviceAndSwapChain).in(LI_MODULE("d3d11.dll").cached())(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, FeatureLevelsRequested,
		sizeof(FeatureLevelsRequested) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
		&sd, &pSwapchain, &pDevice, &FeatureLevelsSupported, &pContext);

	if (FAILED(hr))
		return false;

	// Get VMT addresses
	void** pSwapchainVMT = *(void***)pSwapchain;
	//void** pDeviceVMT = *(void***)pDevice;
	//void** pDeviceContextVMT = *(void***)pContext;

	// Free our pointers, as we will be using the taget's now
	if (pSwapchain) { pSwapchain->Release(); pSwapchain = nullptr; }
	if (pDevice) { pDevice->Release(); pDevice = nullptr; }
	if (pContext) { pContext->Release(); pContext = nullptr; }

#ifdef USEMINHOOK
	if (MH_Initialize() != MH_OK) { return false; }
	if (MH_CreateHook((DWORD_PTR*)pSwapchainVMT[8], PresentHook, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) { return false; }
	if (MH_EnableHook((DWORD_PTR*)pSwapchainVMT[8]) != MH_OK) { return false; }
#else

	FHPresent = FuncHook((uintptr_t)pSwapchainVMT[8], (uintptr_t)PresentHook);
	FHPresent.Hook();

#endif
#endif

	LOG("Swapchain hooked");

	// List of VMTs https://github.com/guided-hacking/GH_D3D11_Hook/blob/master/GH_D3D11_Hook/D3D_VMT_Indices.h

	//if (MH_CreateHook((DWORD_PTR*)pDeviceContextVMT[12], DrawIndexedHook, reinterpret_cast<void**>(&phookD3D11DrawIndexed)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pDeviceContextVMT[12]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pDeviceVMT[24], hookD3D11CreateQuery, reinterpret_cast<void**>(&phookD3D11CreateQuery)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pDeviceVMT[24]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pDeviceContextVMT[8], hookD3D11PSSetShaderResources, reinterpret_cast<void**>(&phookD3D11PSSetShaderResources)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pDeviceContextVMT[8]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pSwapchainVMT[50], ClearRenderTargetViewHook, reinterpret_cast<void**>(&phookD3D11ClearRenderTargetViewHook)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pSwapchainVMT[50]) != MH_OK) { return 1; }

	return true;
}

void Hooks::Release()
{
	Globals::bMenuOpen = false;

	/*::ImGui_ImplDX11_Shutdown();
	::ImGui_ImplWin32_Shutdown();
	::ImGui::DestroyContext();*/

#ifdef USEMINHOOK
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
#else

#ifdef DISCORD
	** reinterpret_cast<D3D11PresentHook**>(DiscordPresent) = reinterpret_cast<D3D11PresentHook>(phookD3D11Present);
#else
	FHPresent.UnHook();
#endif

	FHTestFunc.UnHook();
	FHGetDeviceData.UnHook();

	//FHGetCursorPos.UnHook();
#endif

	lowLevelHooks.Release();
	antiInputHooks.Release();

	UnHookOnProcessSpellCast();

	LOG("Hooks unhooked");

	// wait for last hooks to finish
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	imageManager.Unload();

	if (pDevice) { pDevice->Release(); pDevice = nullptr; }
	if (pSwapchain) { pSwapchain->Release(); pSwapchain = nullptr; }
	if (pContext) { pContext->Release(); pContext = nullptr; }
	if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
	if (pBackbuffer) { pBackbuffer->Release(); pBackbuffer = nullptr; }

	if (pDirectInput) { pDirectInput->Release(); pDirectInput = nullptr; }

	LOG("Pointers released");
}

bool Hooks::InitD3D(IDXGISwapChain* pSwapchain)
{
	if (FAILED(pSwapchain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice))))
		return false;

	pDevice->GetImmediateContext(&pContext);

	pContext->OMGetRenderTargets(1, &pRenderTargetView, nullptr);

	if (!pRenderTargetView)
	{
		if (FAILED(pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackbuffer)))
			return false;

		pDevice->CreateRenderTargetView(pBackbuffer, nullptr, &pRenderTargetView);
		pBackbuffer->Release();

		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	}

	InitImages(pDevice);

	LOG("Success");

	return true;
}

bool Hooks::InitImgui(IDXGISwapChain* pSwapchain)
{
	::ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.Fonts->AddFontDefault();

	// https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp
	ImFontConfig fontConfig;
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(cascadiamono_compressed_data_base85, 13.f, &fontConfig);
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(Tahoma_compressed_data_base85, 13.f, &fontConfig);
	Globals::pDefaultFont = io.Fonts->Fonts[0];

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	// todo style
	ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;

	/*DXGI_SWAP_CHAIN_DESC Desc;
	pSwapchain->GetDesc(&Desc);
	HWND WindowHwnd = Desc.OutputWindow;*/

	::ImGui_ImplWin32_Init(Globals::Hwnd /*WindowHwnd*/);
	::ImGui_ImplDX11_Init(pDevice, pContext);
	::ImGui_ImplDX11_CreateDeviceObjects();

	LOG("Success");

	return true;
}

bool Hooks::InitDInput()
{
#define DEFINE_GUID2(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

	DEFINE_GUID2(GUID_Key, 0x55728220, 0xD33C, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
	DEFINE_GUID2(GUID_SysMouse, 0x6F1D2B60, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
	DEFINE_GUID2(GUID_SysKeyboard, 0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
	DEFINE_GUID2(GUID_Xbox360Controller, 0x028E045E, 0x0000, 0x0000, 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44);

	IID IID_IDirectInput8A = { 0xBF798030, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00 };
	IID IID_IDirectInput8W = { 0xBF798031, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00 };

	HRESULT hr = LI_FN(DirectInput8Create).in(LI_MODULE("DINPUT8.dll").cached())((HINSTANCE)Globals::hModule, DIRECTINPUT_VERSION, IID_IDirectInput8W, (LPVOID*)&pDirectInput, NULL);

	if (FAILED(hr))
		return false;

	LPDIRECTINPUTDEVICE8  lpDirectInputDevice;
	if (pDirectInput->CreateDevice(GUID_SysKeyboard, &lpDirectInputDevice, NULL) != DI_OK)
	{
		pDirectInput->Release();
		return false;
	}

	if (pDirectInput->CreateDevice(GUID_SysMouse, &lpDirectInputDevice, NULL) != DI_OK)
	{
		pDirectInput->Release();
		return false;
	}

	void** lpDirectInputDeviceVMT = *(void***)lpDirectInputDevice;
	//LOG("lpdiKeyboard: %#04x", lpdiKeyboard);
	//LOG("lpdiKeyboardVMT: %#04x", lpdiKeyboardVMT);
	//LOG("lpdiKeyboardVMT[10]: %#04x", lpdiKeyboardVMT[10]);

	if (lpDirectInputDevice)
	{
		lpDirectInputDevice->Unacquire();
		lpDirectInputDevice->Release();
		lpDirectInputDevice = nullptr;
	}

#ifdef USEMINHOOK
	if (MH_CreateHook((DWORD_PTR*)lpDirectInputDeviceVMT[10], GetDeviceDataHook, reinterpret_cast<void**>(&phookGetDeviceData)) != MH_OK) { return false; }
	if (MH_EnableHook((DWORD_PTR*)lpDirectInputDeviceVMT[10]) != MH_OK) { return false; }
#else
	//// todo fix the hooking method, most likely vtable function swap, like minhook
	//// shadow vmt hook wont work, because by calling DirectInput8Create we are getting
	//// a temporary device that points to the original and swapping its vmt wont swap the
	//// original game vmt
	//void** lpdiKeyboardVMT = *(void***)lpdiKeyboard;
	///phookGetDeviceData = reinterpret_cast<IDirectInputDeviceGetDeviceDataHook>(VFHGetDeviceDataHook.Hook2(lpdiKeyboard, 10, &GetDeviceDataHook));

	FHGetDeviceData = FuncHook((uintptr_t)lpDirectInputDeviceVMT[10], (uintptr_t)GetDeviceDataHook);
	FHGetDeviceData.Hook();
#endif

	LOG("Success");

	return true;
}

bool Hooks::InitImages(ID3D11Device* pDevice)
{
	imageManager.SetDevice(pDevice);

	imageManager.AddImage(XorStr("aatrox_square"), Images::aatrox_square, sizeof(Images::aatrox_square));

	imageManager.AddImage(XorStr("talon_square"), Images::talon_square_0, sizeof(Images::talon_square_0));
	imageManager.AddImage(XorStr("talon_w"), Images::talonw, sizeof(Images::talonw));
	imageManager.AddImage(XorStr("talon_e"), Images::talone, sizeof(Images::talone));
	imageManager.AddImage(XorStr("talon_r"), Images::talonr, sizeof(Images::talonr));

	imageManager.AddImage(XorStr("xpbar"), Images::xpbar, sizeof(Images::xpbar));
	imageManager.AddImage(XorStr("xpbarblack"), Images::xpbarblack, sizeof(Images::xpbarblack));
	imageManager.AddImage(XorStr("hudicon"), Images::hudicon, sizeof(Images::hudicon));
	imageManager.AddImage(XorStr("hudcircle"), Images::hudcircle, sizeof(Images::hudcircle));
	imageManager.AddImage(XorStr("hudleft"), Images::hudleft, sizeof(Images::hudleft));
	imageManager.AddImage(XorStr("manabar"), Images::manabar, sizeof(Images::manabar));
	imageManager.AddImage(XorStr("hpbar"), Images::hpbar, sizeof(Images::hpbar));
	imageManager.AddImage(XorStr("hpbarblack"), Images::hpbarblack, sizeof(Images::hpbarblack));

	imageManager.AddImage(XorStr("summoner_flash"), Images::summoner_flash, sizeof(Images::summoner_flash));
	imageManager.AddImage(XorStr("summoner_heal"), Images::summoner_heal, sizeof(Images::summoner_heal));

	LOG("Success");

	return true;
}

//
//void __stdcall DrawIndexedHook(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
//{
//	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
//}
//
//void __stdcall hookD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
//{
//	if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
//	{
//		D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
//		(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
//		(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;
//
//		return phookD3D11CreateQuery(pDevice, &oqueryDesc, ppQuery);
//	}
//
//	return phookD3D11CreateQuery(pDevice, pQueryDesc, ppQuery);
//}
//
//UINT pssrStartSlot;
//D3D11_SHADER_RESOURCE_VIEW_DESC Descr;
//
//void __stdcall hookD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
//{
//	pssrStartSlot = StartSlot;
//
//	for (UINT j = 0; j < NumViews; j++)
//	{
//		ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[j];
//		if (pShaderResView)
//		{
//			pShaderResView->GetDesc(&Descr);
//
//			if ((Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFER) || (Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFEREX))
//			{
//				continue; //Skip buffer resources
//			}
//		}
//	}
//
//	return phookD3D11PSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);
//}
//
//void __stdcall ClearRenderTargetViewHook(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
//{
//	return phookD3D11ClearRenderTargetViewHook(pContext, pRenderTargetView, ColorRGBA);
//}