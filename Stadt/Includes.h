#pragma once

#include "pch.h"

#include "XorString.h"

#include "Definitions.h"

#include "json.h"
#include "lazy_importer.h"

//#define USEMINHOOK

#ifdef USEMINHOOK
#include "MinHook.h"
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#include "Offsets.h"
#include "XorString.h"
#include "Globals.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <tchar.h>
#include <dinput.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <dcomp.h>
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "d3d11.lib")
