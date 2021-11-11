#pragma once

#include <Windows.h>

#include <imgui.h>

namespace Globals
{
	extern HANDLE hModule;
	extern DWORD dwBaseAddress;
	extern WNDPROC WndProc;
	extern HWND Hwnd;
	extern DWORD dwPid;
	extern bool bMenuOpen;
	extern ImFont* pDefaultFont;
	extern float fGameTime;
}