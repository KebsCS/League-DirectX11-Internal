// todo, try this: https://github.com/dencelle/Vault21/blob/1e0f23d5ef18a42b0b09bc74a1ca40410c822c49/V21/VirtualTableManager.cpp

//#pragma once
//
//#include <Windows.h>
//#include <map>
//
//#include "Ntfuncs.h"
//
//#include "Console.h"
//
//
//class VFuncHook
//{
//private:
//	void** originalVMT;
//	std::map<int, void*> hookedFuncs;
//
//public:
//
//	void* HookIndex(void* VMT, size_t index, void* func)
//	{
//		if (!VMT)
//			return 0;
//
//		originalVMT = (void**)VMT;
//
//		hookedFuncs.insert({ index,originalVMT[index] });
//
//		DWORD dwOld;
//		SIZE_T size = sizeof(uintptr_t);
//		FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, PAGE_EXECUTE_READWRITE, &dwOld);
//		originalVMT[index] = func;
//		FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, dwOld, &dwOld);
//
//
//		return hookedFuncs[index];
//	}
//
//	bool UnHook()
//	{
//		for (auto& f : hookedFuncs)
//		{
//			DWORD dwOld;
//			SIZE_T size = sizeof(uintptr_t);
//			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[f.first]), &size, PAGE_EXECUTE_READWRITE, &dwOld);
//			originalVMT[f.first] = f.second;
//			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[f.first]), &size, dwOld, &dwOld);
//		}
//		hookedFuncs.clear();
//		originalVMT = nullptr;
//	}
//
//};
//
//// todo unhooking, by storing hooked function ids, or just 1 func, and move defs to .cpp
////class VFuncHook
////{
////private:
////	uintptr_t originalFunc;
////	//uintptr_t* copiedVMT = nullptr;
////	size_t iMethods = 0;
////	bool bEnabled = false;
////
////public:
////
////	VFuncHook(void* VMT = nullptr)
////	{
////		if (VMT)
////			Init(VMT);
////	}
////	bool Init(void* VMT)
////	{
////		if (VMT)
////		{
////			originalFunc = (uintptr_t)VMT;
////		}
////		return false;
////	}
////
////	uintptr_t HookIndex(const size_t& index, const uintptr_t& func)
////	{
////
////
////			DWORD dwOld;
////			SIZE_T size = sizeof(uintptr_t);
////			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, PAGE_EXECUTE_READWRITE, &dwOld);
////			originalVMT[index] = func;
////			FVirtualProtectEx((HANDLE)0xFFFFFFFF, &(originalVMT[index]), &size, dwOld, &dwOld);
////			bEnabled = true;
////
////			return originalFunc;
////
////	}
////
////	uintptr_t Hook(void* original, const size_t& index, const uintptr_t& function)
////	{
////		if (!Init(original))
////			return false;
////
////		return HookIndex(index, function);
////	}
////
////	bool UnHook()
////	{
////		if (bEnabled)
////		{
////			bEnabled = false;
////			return true;
////		}
////		return false;
////	}
////};
