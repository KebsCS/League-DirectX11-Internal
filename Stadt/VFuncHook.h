#pragma once

#include <Windows.h>

#include "Ntfuncs.h"

// todo unhooking, by storing hooked function ids, or just 1 func, and move defs to .cpp
class VFuncHook
{
private:
	uintptr_t* originalVMT = nullptr;
	//uintptr_t* copiedVMT = nullptr;
	size_t iMethods = 0;
	bool bEnabled = false;

public:

	VFuncHook(void* VMT = nullptr)
	{
		if (VMT)
			Init(VMT);
	}
	bool Init(void* VMT)
	{
		if (VMT)
		{
			originalVMT = reinterpret_cast<uintptr_t*>(VMT);
			while (reinterpret_cast<void*>(originalVMT[iMethods]))
			{
				iMethods++;
			}

			if (iMethods > 0)
				return true;
		}
		return false;
	}

	uintptr_t HookIndex(size_t index, void* func)
	{
		if (index < iMethods)
		{
			uintptr_t originalFunc = originalVMT[index];

			DWORD dwOld;
			SIZE_T size = sizeof(uintptr_t);
			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, PAGE_EXECUTE_READWRITE, &dwOld);
			originalVMT[index] = reinterpret_cast<uintptr_t>(func);
			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, dwOld, &dwOld);
			bEnabled = true;

			return originalFunc;
		}
		return 0;
	}

	uintptr_t Hook(void* original, size_t index, void* function)
	{
		if (!Init(original))
			return false;

		return HookIndex(index, function);
	}

	bool UnHook()
	{
		if (bEnabled)
		{
			bEnabled = false;
			return true;
		}
		return false;
	}
};
