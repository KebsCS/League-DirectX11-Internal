#pragma once

#include "Includes.h"

#define SCREENWIDTH ::GetSystemMetrics(SM_CXSCREEN)
#define SCREENHEIGHT ::GetSystemMetrics(SM_CYSCREEN)

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
	bool Init();
	void Release();
	HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	bool InitD3D(IDXGISwapChain* pSwapchain);
	bool InitImgui(IDXGISwapChain* pSwapchain);
};
