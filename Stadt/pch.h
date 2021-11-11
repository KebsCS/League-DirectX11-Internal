#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
#define _WIN32_WINNT						_WIN32_WINNT_WIN10
#define WINVER								_WIN32_WINNT_WIN10

#define NOMINMAX

#ifdef _DEBUG
//#error Compile in Release mode
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
