#pragma once

#include "pch.h"

#include "stb_image.h"

#include "XorString.h"
#include "Console.h"

#include "Definitions.h"

#include "json.h"

//#define LAZY_IMPORTER_CASE_INSENSITIVE
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

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"