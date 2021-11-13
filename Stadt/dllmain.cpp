// todo move this to images.cpp
#define STB_IMAGE_IMPLEMENTATION

#include "Includes.h"
#include "Hooks.h"
#include "Ntdefs.h"
#include "Ntfuncs.h"

struct HandleData
{
	DWORD pid;
	HWND hWnd;
};

BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
	HandleData& data = *(HandleData*)lParam;
	DWORD pid = 0;
	LI_FN(GetWindowThreadProcessId)(hWnd, &pid);
	if (pid == data.pid && LI_FN(GetWindow)(hWnd, GW_OWNER) == HWND(0) && LI_FN(IsWindowVisible)(hWnd))
	{
		data.hWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

HWND FindMainWindow(DWORD dwPID)
{
	HandleData handleData{ 0 };
	handleData.pid = dwPID;
	LI_FN(EnumWindows)(EnumWindowsCallback, (LPARAM)&handleData);
	return handleData.hWnd;
}

void OnExit() noexcept
{
	// restore original wndproc
	LI_FN(SetWindowLongW)(Globals::Hwnd, GWLP_WNDPROC, (LONG_PTR)Globals::WndProc);

	// delete hooks and d3d
	Hooks::Release();
}

__declspec(safebuffers)void WINAPI InitThread(HMODULE hModule) noexcept
{
	Globals::hModule = hModule;
	Globals::dwBaseAddress = reinterpret_cast<DWORD>(GetModuleBase(nullptr));
	Globals::dwPid = GetCurrentPID();
	Globals::Hwnd = FindMainWindow(Globals::dwPid);
	Globals::WndProc = reinterpret_cast<WNDPROC>(LI_FN(SetWindowLongW)(Globals::Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc));

	/*if (!*(DWORD*)RVA(oLocalPlayer) && *(float*)RVA(oGameTime) < 1.f)
	{
		MessageBoxA(0, "Outdated offsets", "Error", 0);
	}*/

	if (Hooks::Init())
	{
		while (!LI_FN(GetAsyncKeyState).get()(VK_END))
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	OnExit();

	LI_FN(FreeLibraryAndExitThread).get()(hModule, ERROR_SUCCESS);
}

#define THREAD_ALL_ACCESS_VISTA         (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
	0xFFFF)

#define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
#define THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH 0x00000002
#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER 0x00000004
#define THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR 0x00000010
#define THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET 0x00000020
#define THREAD_CREATE_FLAGS_INITIAL_THREAD 0x00000080

HANDLE hThread = 0;

//#pragma comment(linker, "/nodefaultlib /subsystem:windows /ENTRY:DllMain")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	LI_FN(DisableThreadLibraryCalls).get()(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		ULONG flags;
#ifdef _DEBUG
		flags = 0;
#else
		flags = THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER;
#endif
		// todo maybe without a thread like in https://github.com/reahly/valorant-internal/blob/main/vitasnella-valorant/entrypoint.cpp#L25
		NTSTATUS ntStatus = FNtCreateThreadEx(&hThread, THREAD_ALL_ACCESS_VISTA, 0, ((HANDLE)(LONG_PTR)-1), (LPTHREAD_START_ROUTINE)InitThread, hModule, flags, 0, 0, 0, 0);
		if (ntStatus >= 0) // success
		{
			if (hThread != INVALID_HANDLE_VALUE)
			{
				LI_FN(CloseHandle).safe()(hThread);
			}
		}
		//CloseHandle((HANDLE)_beginthreadex(nullptr, 0, reinterpret_cast<_beginthreadex_proc_type>(InitThread), hModule, 0, nullptr));
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		OnExit();
		LI_FN(FreeLibrary).get()(hModule);
		if (hThread != INVALID_HANDLE_VALUE)
		{
			// todo very rarely crashes on unload, "invalid handle was specified"
			// or "NtClose was called on a handle that was protected from close via NtSetInformationOnject"
			try
			{
				//LI_FN(CloseHandle).safe()(hThread);
			}
			catch (...) {};
		}
		break;
	}
	return TRUE;
}