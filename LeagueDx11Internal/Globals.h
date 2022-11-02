#pragma once

#include <Windows.h>

#include <imgui.h>

#include "Matrix4x4.h"

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
	extern Matrix4x4 viewMatrix;
	extern Matrix4x4 projectionMatrix;
	extern float multipliedMatrix[16];
}