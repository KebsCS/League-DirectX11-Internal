#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define STRICT

#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
#define _WIN32_WINNT						_WIN32_WINNT_WIN10
#define WINVER								_WIN32_WINNT_WIN10

#define NOMINMAX

#ifdef _DEBUG
#error Compile in Release mode
#else
// disable exceptions caused by stl
#define _HAS_EXCEPTIONS 0
#endif
#ifdef _WIN64
#error Compile in x86
#endif

// Windows Header Files
#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <sstream>
#include <numbers>
#include <queue>
#include <algorithm>
#include <cmath>

#define DIRECTINPUT_VERSION 0x0800
#include <d3d11.h>
#include <tchar.h>
#include <dinput.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <dcomp.h>
//#include <D3DX11tex.h>
//#pragma comment(lib, "dcomp.lib")
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dinput8")
//#pragma comment(lib, "dxguid")
//#pragma comment(lib, "D3DX11.lib")
