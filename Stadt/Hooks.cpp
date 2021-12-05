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

#include "FuncHook.h"
#include "VMTHook.h"

typedef HRESULT(WINAPI* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
D3D11PresentHook				phookD3D11Present = nullptr;
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
typedef HRESULT(WINAPI* IDirectInputDeviceGetDeviceDataHook)(IDirectInputDevice8A* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
IDirectInputDeviceGetDeviceDataHook phookGetDeviceData = nullptr;

ID3D11Device* pDevice = nullptr;
IDXGISwapChain* pSwapchain = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
ID3D11Texture2D* pBackbuffer = nullptr;

IDirectInput8A* pDirectInput = nullptr;

// move this?
FuncHook FHPresent;
FuncHook FHGetDeviceData;
VMTHook VMTHOnProcessSpellCast[10]; // todo, maybe a vector or list

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// hotkeys
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_INSERT)
			Globals::bMenuOpen = !Globals::bMenuOpen;
	}

	if (Globals::bMenuOpen)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;
	}

	return LI_FN(CallWindowProcW).get()(Globals::WndProc, hwnd, uMsg, wParam, lParam);
}

static HRESULT WINAPI Hooks::GetDeviceDataHook(IDirectInputDevice8A* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	// Call original function to get actual keypresses
#ifdef USEMINHOOK
	HRESULT ret = phookGetDeviceData(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);
#else
	// fix hooking method
	//HRESULT ret = FHGetDeviceData.Call<HRESULT>(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);
	HRESULT ret = 0;//phookGetDeviceData(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);

#endif

	if (ret == DI_OK)
	{
		for (DWORD i = 0; i < *pdwInOut; i++)
		{
			if (LOBYTE(rgdod[i].dwData) > 0)
			{
				switch (rgdod[i].dwOfs)
				{
					// blocks W key by changing key code to 0
				case DIK_W:
					rgdod[i].dwOfs = 0;
					break;
				}
			}
		}
	}

	return ret;
}

static std::vector<Vector3>testpos;
static std::vector<Geometry::Polygon>testpoly;

// todo move this
uintptr_t pOnProcessSpellCast;
DWORD* __fastcall OnProcessSpellCast(void* thisptr, void* edx, int state, SpellCastInfo* spellCastInfo, int a6)
{
	static auto fn = reinterpret_cast<DWORD * (__thiscall*)(void*, int, SpellCastInfo*, int)>(pOnProcessSpellCast);

	if (spellCastInfo != nullptr)
	{
		// todo an event manager
		std::string name = spellCastInfo->spellInfo->name;
		//ImVec2 startpos = LeagueFuncs::WorldToScreen(spellCastInfo->startPos);
		//ImVec2 endpos = LeagueFuncs::WorldToScreen(spellCastInfo->endPos);
		//Vector3 startpos = spellCastInfo->StartPos;
		std::stringstream ss;
		ss << std::hex << name << "  " << (DWORD)spellCastInfo << " ";
		/*<< startpos.x << " , " << startpos.y << " endpos: " << endpos.x << " , " << endpos.y;*/

		//testpos.emplace_back(spellCastInfo->endPos);

		GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));
		ss << "  id: " << std::dec << spellCastInfo->slotId;

		/*if (Globals::bMenuOpen)
		{
			std::stringstream ssAddy;
			ssAddy << name << "  " << std::hex << (DWORD)(spellCastInfo);
			MessageBoxA(0, ssAddy.str().c_str(), "", 0);
		}*/

		//ss << "  " << local->name << "  " << &local->networkId;
		if (name == "ThreshQ")
		{
			Vector3 extended = spellCastInfo->startPos.Extend(spellCastInfo->endPos, spellCastInfo->spellInfo->spellData->castRangeDisplayOverride);
			Vector3 sp = spellCastInfo->startPos;
			//extended.y = 100;
			//sp.y = 100;
			Geometry::Polygon poly = Geometry::Rectangle(sp, extended, 70.f).ToPolygon();

			testpoly.emplace_back(poly);
		}
		LeagueFuncs::SendChat(ss.str().c_str());
	}

	return fn(thisptr, state, spellCastInfo, a6);
}

static HRESULT WINAPI Hooks::PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static std::once_flag isInitialized;

	std::call_once(isInitialized, [&]()
		{
			InitD3D(pSwapChain);
			InitImgui(pSwapChain);

			InitDInput();

			for (int i = 0; auto hero : ObjectManager::HeroList())
			{
				DWORD dwOnProcessSpell = (DWORD)hero + 0xE6C;
				//SendChat(std::to_string(dwOnProcessSpell).c_str());
				pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 29, (uintptr_t)&OnProcessSpellCast);
				i++;
			}
		});

	// todo sometimes on unloading pContext was nullptr, maybe make a mutex of some sort
	if (pContext)
	{
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

		::ImGui_ImplDX11_NewFrame();
		::ImGui_ImplWin32_NewFrame();
		::ImGui::NewFrame();

		if (Globals::bMenuOpen)
		{
			::ImGui::ShowDemoWindow();
			::ImGui::ShowMetricsWindow();

			render.RenderScene();

			// VISUALS GO HERE
			// -----

			render.Box(20, 100, 100, 200, ImColor(1.f, 0.f, 0.f));

			ImGuiIO& io = ImGui::GetIO();
			ImVec2 mpos = io.MousePos;
			//render.Text("xddd", mpos.x, mpos.y, 20.f, ImColor(1.f, 0.f, 0.f), true, true);
			//Image aatroxe = imageManager.GetImageInfoByName("aatrox_square");
			//render.Image(mpos.x, mpos.y, aatroxe.width, aatroxe.height, aatroxe.pShaderResource, true);

			render.ImageBordered(10, 10, 64, 64, imageManager.GetImageByName(XorStr("talon_w")), true);

			render.CornerBox(300, 300, 400, 400, ImColor(0.f, 1.f, 0.f));

			static float cd = 200.f;
			if (cd < -200.f)
				cd = 200.f;

			render.FancyIcon(150, 150, XorStr("talon"), cd / 200.f, cd / 200.f, cd / 200.f, 1, cd, XorStr("summoner_flash"), cd, XorStr("summoner_heal"), cd);
			cd -= 1.f;

			for (auto& pos : testpos)
			{
				Vector2 pos2d = LeagueFuncs::WorldToScreen(pos);
				render.Circle(pos2d.x, pos2d.y, 32, 16, ImColor(1.f, 0.f, 0.f));
			}

			/*for (auto& poly : testpoly)
				render.Polygon(poly, ImColor(0.f, 0.f, 1.f));*/

			auto danger_polygons = Geometry::Geometry::ToPolygons(Geometry::Geometry::ClipPolygons(testpoly));

			for (auto& poly : danger_polygons)
				render.Polygon(poly, ImColor(0.f, 0.f, 1.f));

			for (auto h : ObjectManager::HeroList())
				render.Circle3D(h->GetAiManager()->serverPos, h->GetBoundingRadius(), ImColor(0.f, 1.f, 0.f));

			ImColor mousecol;
			if (LeagueFuncs::IsBrush(LeagueFuncs::GetMouseWorldPos()))
				mousecol = ImColor(1.f, 0.f, 0.f);
			else
				mousecol = ImColor(0.f, 1.f, 0.f);

			render.Circle3D(LeagueFuncs::GetMouseWorldPos(), 65.f, mousecol);
			GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));

			//std::vector<Vector3>points = *reinterpret_cast<std::vector<Vector3>*>(local->GetAiManager() + 0x1E4);

			//Vector3 points = **reinterpret_cast<Vector3**>(local->GetAiManager() + 0x1E4);
			//DWORD arr = *reinterpret_cast<DWORD*>(local->GetAiManager() + 0x1E4);
			//Vector3 points = *reinterpret_cast<Vector3*>(arr);
			//render.Circle3D(points, 30.f, ImColor(1.f, 1.f, 1.f));

			Vector3 waypoint = local->GetAiManager()->navBegin;

			render.Circle3D(waypoint, local->GetBoundingRadius(), ImColor(1.f, 1.f, 0.f));

			for (auto minion : ObjectManager::MinionList())
			{
				if (minion->maxhealth == 3.f && minion->health > 0.f)
					render.RealWardRange(minion->position, ImColor(1.f, 1.f, 1.f));

				if (minion && minion->health > 0.f && minion->GetBoundingRadius())
				{
					render.Circle3D(minion->position, minion->GetBoundingRadius(), ImColor(1.f, 0.f, 0.f));
				}
			}

			render.Circle3D(local->position, local->GetBoundingRadius() - 5.f, ImColor(1.f, 0.f, 0.f));

			Menu::Render();
		}
		else
		{
			testpos.clear();
			testpoly.clear();
		}
		// -----

		::ImGui::EndFrame();
		::ImGui::Render();

		::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

#ifdef USEMINHOOK
	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
#else
	return FHPresent.Call<HRESULT>(pSwapChain, SyncInterval, Flags);
#endif
}

bool Hooks::Init()
{
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
	void** pDeviceVMT = *(void***)pDevice;
	void** pDeviceContextVMT = *(void***)pContext;

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

	FHPresent.UnHook();
#endif

	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();

	// wait for last hooks to finish
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	imageManager.Unload();

	if (pDevice) { pDevice->Release(); pDevice = nullptr; }
	if (pSwapchain) { pSwapchain->Release(); pSwapchain = nullptr; }
	if (pContext) { pContext->Release(); pContext = nullptr; }
	if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
	if (pBackbuffer) { pBackbuffer->Release(); pBackbuffer = nullptr; }

	if (pDirectInput) { pDirectInput->Release(); pDirectInput = nullptr; }
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

	HRESULT hr = LI_FN(DirectInput8Create).in(LI_MODULE("DINPUT8.dll").cached())((HINSTANCE)Globals::hModule, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&pDirectInput, NULL);

	if (FAILED(hr))
		return false;

	LPDIRECTINPUTDEVICE8A  lpdiKeyboard;
	if (pDirectInput->CreateDevice(GUID_SysKeyboard, &lpdiKeyboard, NULL) != DI_OK)
	{
		pDirectInput->Release();
		return false;
	}

	void** lpdiKeyboardVMT = *(void***)lpdiKeyboard;

	if (lpdiKeyboard)
	{
		lpdiKeyboard->Unacquire();
		lpdiKeyboard->Release();
		lpdiKeyboard = nullptr;
	}

#ifdef USEMINHOOK

	if (MH_CreateHook((DWORD_PTR*)lpdiKeyboardVMT[10], GetDeviceDataHook, reinterpret_cast<void**>(&phookGetDeviceData)) != MH_OK) { return false; }
	if (MH_EnableHook((DWORD_PTR*)lpdiKeyboardVMT[10]) != MH_OK) { return false; }
#else
	//// todo fix the hooking method, most likely vtable function swap, like minhook
	//// shadow vmt hook wont work, because by calling DirectInput8Create we are getting
	//// a temporary device that points to the original and swapping its vmt wont swap the
	//// original game vmt
	//void* pVMT = lpdiKeyboard;
	//phookGetDeviceData = reinterpret_cast<IDirectInputDeviceGetDeviceDataHook>(VMTHGetDeviceData.Hook(pVMT, 10, (uintptr_t)&GetDeviceDataHook));
#endif

	return true;
}

bool Hooks::InitImages(ID3D11Device* pDevice)
{
	imageManager.SetDevice(pDevice);

	imageManager.AddImage(XorStr("aatrox_square"), (char*)&Images::aatrox_square, sizeof(Images::aatrox_square));

	imageManager.AddImage(XorStr("talon_square"), (char*)&Images::talon_square_0, sizeof(Images::talon_square_0));
	imageManager.AddImage(XorStr("talon_w"), (char*)&Images::talonw, sizeof(Images::talonw));
	imageManager.AddImage(XorStr("talon_e"), (char*)&Images::talone, sizeof(Images::talone));
	imageManager.AddImage(XorStr("talon_r"), (char*)&Images::talonr, sizeof(Images::talonr));

	imageManager.AddImage(XorStr("xpbar"), (char*)&Images::xpbar, sizeof(Images::xpbar));
	imageManager.AddImage(XorStr("xpbarblack"), (char*)&Images::xpbarblack, sizeof(Images::xpbarblack));
	imageManager.AddImage(XorStr("hudicon"), (char*)&Images::hudicon, sizeof(Images::hudicon));
	imageManager.AddImage(XorStr("hudcircle"), (char*)&Images::hudcircle, sizeof(Images::hudcircle));
	imageManager.AddImage(XorStr("hudleft"), (char*)&Images::hudleft, sizeof(Images::hudleft));
	imageManager.AddImage(XorStr("manabar"), (char*)&Images::manabar, sizeof(Images::manabar));
	imageManager.AddImage(XorStr("hpbar"), (char*)&Images::hpbar, sizeof(Images::hpbar));
	imageManager.AddImage(XorStr("hpbarblack"), (char*)&Images::hpbarblack, sizeof(Images::hpbarblack));

	imageManager.AddImage(XorStr("summoner_flash"), (char*)&Images::summoner_flash, sizeof(Images::summoner_flash));
	imageManager.AddImage(XorStr("summoner_heal"), (char*)&Images::summoner_heal, sizeof(Images::summoner_heal));

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