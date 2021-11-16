#pragma once

#include "Includes.h"

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
	bool Init();
	void Release();
	HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	bool InitD3D(IDXGISwapChain* pSwapchain);
	bool InitImgui(IDXGISwapChain* pSwapchain);
	bool InitImages(ID3D11Device* pDevice);
	bool InitDInput();
};
