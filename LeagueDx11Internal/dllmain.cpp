#define STB_IMAGE_IMPLEMENTATION


#include "Includes.h"
#include "Hooks.h"
#include "Ntdefs.h"
#include "Ntfuncs.h"
#include "SelfProtection.h"

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

	Console::Exit();
}

__declspec(safebuffers)void WINAPI InitThread(HMODULE hModule) noexcept
{
	Globals::dwBaseAddress = reinterpret_cast<DWORD>(GetModuleBase(nullptr));
	Globals::hModule = reinterpret_cast<HANDLE>(Globals::dwBaseAddress);
	Globals::dwPid = GetCurrentPID();
	Globals::Hwnd = FindMainWindow(Globals::dwPid);
	Globals::WndProc = reinterpret_cast<WNDPROC>(LI_FN(SetWindowLongW)(Globals::Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc));

	LI_FN(srand)((unsigned int)time(NULL));

	Console::Init();

	SelfProtection protection;
	bool bFailed = false;
#ifndef DEVELOPER
	if (protection.InitializeAntiDump(hModule) == false)
	{
		LOG("Anti dump failed");
		bFailed = true;
	}
	protection.HideModuleLinks(hModule);
	protection.ProtectSelfPE(hModule);
#endif
	/*if (protection.InitializeAntiDump(hModule) == false)
	{
		LOG("Anti dump failed");
	}
	protection.HideModuleLinks(hModule);
	protection.ProtectSelfPE(hModule);*/

	if (!*(DWORD*)RVA(oLocalPlayer) && *(float*)RVA(oGameTime) < 1.f)
	{
		LOG("Outdated offsets");
		bFailed = true;
	}

	std::array<std::uint8_t, 2> gadget = *(std::array<std::uint8_t, 2>*)RVA(oSpoofGadget);
	if (gadget.at(0) != 0xFF || gadget.at(1) != 0x23)
	{
		LOG("Wrong spoof gadget");
		bFailed = true;
	}
	LOG("%x %x", gadget.at(0), gadget.at(1));

	if (Hooks::Init() && !bFailed)
	{
		while (!LI_FN(GetAsyncKeyState).get()(VK_END))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	OnExit();

	LI_FN(FreeLibraryAndExitThread).get()(hModule, ERROR_SUCCESS);
}



//#pragma comment(linker, "/nodefaultlib:YES /subsystem:windows /ENTRY:DllMain1")

//__declspec(dllexport) LRESULT CALLBACK OurCallbackFunction(int code, WPARAM wparam, LPARAM lparam) {
//	MSG* msg = (MSG*)lparam;
//
//	if (msg->message == (WM_USER + 432)) {
//				ULONG flags;
//#ifdef _DEBUG
//		flags = 0;
//#else
//		flags = THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER;
//#endif
//		// todo maybe without a thread like in https://github.com/reahly/valorant-internal/blob/main/vitasnella-valorant/entrypoint.cpp#L25
//		NTSTATUS ntStatus = FNtCreateThreadEx(&hThread, THREAD_ALL_ACCESS_VISTA, 0, ((HANDLE)(LONG_PTR)-1), (LPTHREAD_START_ROUTINE)InitThread, 0/*hModule*/, flags, 0, 0, 0, 0);
//		if (ntStatus >= 0) // success
//		{
//			if (hThread != INVALID_HANDLE_VALUE)
//			{
//				LI_FN(CloseHandle).safe()(hThread);
//			}
//		}
//	}
//
//	return CallNextHookEx(0, code, wparam, lparam);
//}

HANDLE hThread = 0;
BOOL APIENTRY DllMain1(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
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