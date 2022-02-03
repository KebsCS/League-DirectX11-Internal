#pragma once

#include "Includes.h"
#include "FuncHook.h"
#include "Ntfuncs.h"

// TODO LESS HEAVY HOOKING METHOD

FuncHook FHGetCurrentInputMessageSource;
BOOL WINAPI GetCurrentInputMessageSourceHook(INPUT_MESSAGE_SOURCE* inputMessageSource)
{
	BOOL ret = FHGetCurrentInputMessageSource.Call<BOOL>(inputMessageSource);

	if (inputMessageSource->originId == IMO_INJECTED)
		inputMessageSource->originId = IMO_HARDWARE;

	return ret;
}

FuncHook FHGetRawInputData;
UINT WINAPI GetRawInputDataHook(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader)
{
	UINT ret = FHGetRawInputData.Call<UINT>(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);
	if (uiCommand == RID_INPUT)
	{
		if ((*pcbSize > 0) && pData)
		{
			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(pData);
			//LOG("%d %d %d", pcbSize, *pcbSize, &pcbSize);
			if (raw->header.dwType != RIM_TYPEMOUSE)
				return ret;

			//if ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE)
			//{
			//	//raw->data.mouse.usFlags &= ~MOUSE_MOVE_ABSOLUTE;
			//	//raw->data.mouse.usFlags &= MOUSE_MOVE_RELATIVE;
			//	LOG("deted");
			//}
			LOG("call");
		}
	}

	return ret;
}

class AntiInputHooks
{
private:

public:

	bool Init()
	{
		HookGetCurrentInputMessageSource();
		//HookGetRawInputData(); todo fix crash
		return true;
	}

	bool HookGetCurrentInputMessageSource()
	{
		std::string szUser32 = XorStr("user32.dll");
		std::string szFunc = XorStr("GetCurrentInputMessageSource");
		FHGetCurrentInputMessageSource = FuncHook((uintptr_t)GetProcedureAddress(GetModuleBase(szUser32.c_str()),
			szFunc.c_str()), (uintptr_t)GetCurrentInputMessageSourceHook);
		FHGetCurrentInputMessageSource.Hook();

		// also GetCIMSSM, but it is being depracated
		// and seems to only return unavailable input

		return true;
	}

	//test GetRawInputDeviceInfo I couldnt get it to work
	//Just register a WH_DEBUG and deny their callbacks, or even unregister them.

	//Any good anti-cheat will use GetRawInputData + process WM_INPUT first in wndproc message pump to detect remote input imo, not this.

	bool HookGetRawInputData()
	{
		std::string szUser32 = XorStr("user32.dll");
		std::string szFunc = XorStr("GetRawInputData");
		FHGetRawInputData = FuncHook((uintptr_t)GetProcedureAddress(GetModuleBase(szUser32.c_str()),
			szFunc.c_str()), (uintptr_t)GetRawInputDataHook);
		FHGetRawInputData.Hook();

		return true;
	}

	void Release()
	{
		FHGetCurrentInputMessageSource.UnHook();
		FHGetRawInputData.UnHook();
	}
};