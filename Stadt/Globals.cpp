#include "Globals.h"

namespace Globals
{
	HANDLE hModule = nullptr;
	DWORD dwBaseAddress = 0;
	WNDPROC WndProc;
	HWND Hwnd = nullptr;
	DWORD dwPid = 0;
	bool bMenuOpen = true;
	ImFont* pDefaultFont;
	float fGameTime = 0.f;
}