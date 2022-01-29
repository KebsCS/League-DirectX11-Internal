#pragma once

#include "Includes.h"

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
	bool Init();
	void Release();
	static HRESULT WINAPI GetDeviceDataHook(IDirectInputDevice8W* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
	static HRESULT WINAPI PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	bool InitD3D(IDXGISwapChain* pSwapchain);
	bool InitImgui(IDXGISwapChain* pSwapchain);
	bool InitImages(ID3D11Device* pDevice);
	bool InitDInput();
};
