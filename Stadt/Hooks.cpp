#include "Hooks.h"
#include "Ntdefs.h"
#include "Render.h"
#include "Tahoma.h"
#include "Ntfuncs.h"

#include "FuncHook.h"

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
typedef void(__stdcall* D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

D3D11PresentHook				phookD3D11Present = nullptr;
D3D11DrawIndexedHook            phookD3D11DrawIndexed = nullptr;
D3D11CreateQueryHook			phookD3D11CreateQuery = nullptr;
D3D11PSSetShaderResourcesHook	phookD3D11PSSetShaderResources = nullptr;
D3D11ClearRenderTargetViewHook  phookD3D11ClearRenderTargetViewHook = nullptr;

ID3D11Device* pDevice = nullptr;
IDXGISwapChain* pSwapchain = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
ID3D11Texture2D* pBackbuffer = nullptr;

FuncHook FHPresent;

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

	std::string szd3d11 = XorStr("d3d11.dll");
	std::string szD3D11CreateDeviceAndSwapChain = XorStr("D3D11CreateDeviceAndSwapChain");

	// todo maybe LI_FN instead of ModuleCall
	HRESULT hr = ModuleCall<HRESULT>(szd3d11.c_str(), szD3D11CreateDeviceAndSwapChain.c_str(), nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, FeatureLevelsRequested, sizeof(FeatureLevelsRequested) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &sd, &pSwapchain, &pDevice, &FeatureLevelsSupported, &pContext);
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
	FHPresent.hook();

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
	FHPresent.unhook();
#endif

	// wait for last hooks to finish
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (pDevice) { pDevice->Release(); pDevice = nullptr; }
	if (pSwapchain) { pSwapchain->Release(); pSwapchain = nullptr; }
	if (pContext) { pContext->Release(); pContext = nullptr; }
	if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
	if (pBackbuffer) { pBackbuffer->Release(); pBackbuffer = nullptr; }
}

HRESULT __stdcall Hooks::PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static std::once_flag isInitialized;

	std::call_once(isInitialized, [&]()
		{
			InitD3D(pSwapChain);
			InitImgui(pSwapChain);
		});

	// todo sometimes on unloading pContext was nullptr, maybe make a mutex of some sort
	pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	::ImGui_ImplDX11_NewFrame();
	::ImGui_ImplWin32_NewFrame();
	::ImGui::NewFrame();

	if (Globals::bMenuOpen)
	{
		::ImGui::ShowDemoWindow();
		::ImGui::ShowMetricsWindow();
	}

	render.RenderScene();

	// VISUALS GO HERE
	// -----
	render.Box(20, 100, 100, 200, ImColor(1.f, 0.f, 0.f));

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mpos = io.MousePos;
	render.Text("xddd", mpos.x, mpos.y, 20.f, ImColor(1.f, 0.f, 0.f), true, true);

	render.CornerBox(300, 300, 400, 400, ImColor(0.f, 1.f, 0.f));

	// -----

	::ImGui::EndFrame();
	::ImGui::Render();

	::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

#ifdef USEMINHOOK
	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
#else
	return FHPresent.Call<HRESULT>(pSwapChain, SyncInterval, Flags);
#endif
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

	return true;
}

bool Hooks::InitImgui(IDXGISwapChain* pSwapchain)
{
	::ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.Fonts->AddFontDefault();

	// https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp
	ImFontConfig fontConfig;
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