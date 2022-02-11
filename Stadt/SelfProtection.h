#pragma once

#include "Includes.h"
#include "Ntdefs.h"
#include "Ntfuncs.h"

// https://github.com/mq1n/NoMercy/tree/3a375e27f56fe9eec9c553c641ce0abde2c6b22b/Source/Client/NM_Engine

#ifndef _M_X64

typedef struct LDR_DATA_ENTRY {
	LIST_ENTRY              InMemoryOrderModuleList;
	PVOID                   BaseAddress;
	PVOID                   EntryPoint;
	ULONG                   SizeOfImage;
	UNICODE_STRING          FullDllName;
	UNICODE_STRING          BaseDllName;
	ULONG                   Flags;
	SHORT                   LoadCount;
	SHORT                   TlsIndex;
	LIST_ENTRY              HashTableEntry;
	ULONG                   TimeDateStamp;
} LDR_DATA_ENTRY, * PLDR_DATA_ENTRY;
__declspec(naked) PLDR_DATA_ENTRY GetLdrDataEntry()
{
	__asm
	{
		mov eax, fs: [0x30]
		mov eax, [eax + 0x0C]
		mov eax, [eax + 0x1C]
		retn
	}
}
#endif

BOOL APIENTRY FakeMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	return TRUE;
}

static LPVOID s_pGuardMem = nullptr;

class SelfProtection
{
private:

	ULONG GetRandomInt(ULONG uMin, ULONG uMax)
	{
		if (uMax < (ULONG)0xFFFFFFFF)
			uMax++;

		return (rand() % (uMax - uMin)) + uMin;
	}

	inline LPVOID CreateSafeMemoryPage(DWORD dwRegionSize, DWORD dwProtection)
	{
		LPVOID pMemBase = nullptr;

		__try
		{
			pMemBase = FVirtualAllocEx(NtCurrentProcess, 0, dwRegionSize, MEM_COMMIT | MEM_RESERVE, dwProtection);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}

		return pMemBase;
	}

public:

	void HideModuleLinks(HMODULE hModule)
	{
#ifndef _M_X64
		auto cursor = GetLdrDataEntry();
		while (cursor->BaseAddress)
		{
			cursor = (PLDR_DATA_ENTRY)cursor->InMemoryOrderModuleList.Flink;
			if (cursor->BaseAddress == hModule)
			{
				auto prev = (PLDR_DATA_ENTRY)cursor->InMemoryOrderModuleList.Blink;

				cursor->BaseDllName = prev->BaseDllName;
				cursor->FullDllName = prev->FullDllName;

				cursor->SizeOfImage = 0;
				cursor->EntryPoint = &FakeMain;

				// TODO: unlink from hash table
				cursor->HashTableEntry.Blink->Flink = cursor->HashTableEntry.Flink;
				cursor->HashTableEntry.Flink->Blink = cursor->HashTableEntry.Blink;
			}
		}
#endif
	}

	void ProtectSelfPE(HMODULE hModule)
	{
		auto pIDH = reinterpret_cast<IMAGE_DOS_HEADER*>(hModule);
		if (pIDH->e_magic == IMAGE_DOS_SIGNATURE)
		{
			auto dwOldProtect = 0UL;
			SIZE_T size = sizeof(IMAGE_DOS_HEADER);
			FVirtualProtectEx(NtCurrentProcess, pIDH, &size, PAGE_READWRITE, &dwOldProtect);
			{
				//pIDH->e_magic = 0;
				for (SIZE_T i = 0; i < size; i++)
					*(BYTE*)((BYTE*)pIDH + i) = 0;

				//	auto pINH = reinterpret_cast<IMAGE_NT_HEADERS*>(pIDH + 1);
				//	LOG("%d", pINH->Signature);
				//	pINH->Signature = 0;

				FVirtualProtectEx(NtCurrentProcess, pIDH, &size, dwOldProtect, &dwOldProtect);
			}
		}
	}

	bool InitializeAntiDump(HMODULE hModule)
	{
#define BUFFER_SIZE 0x1000

		for (std::size_t i = 0; i < GetRandomInt(20, 50); i++)
			CreateSafeMemoryPage(BUFFER_SIZE, PAGE_READWRITE);

		s_pGuardMem = CreateSafeMemoryPage(BUFFER_SIZE, PAGE_READWRITE);

		for (std::size_t i = 0; i < GetRandomInt(20, 50); i++)
			CreateSafeMemoryPage(BUFFER_SIZE, PAGE_READWRITE);

		//	auto hTargetModule = g_winapiModuleTable->hBaseModule;
		auto hTargetModule = hModule;

		auto pDOS = (IMAGE_DOS_HEADER*)hTargetModule;
		if (pDOS->e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		auto pINH = (IMAGE_NT_HEADERS*)(pDOS + pDOS->e_lfanew);
		//	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		//		return false;

		auto pISH = (PIMAGE_SECTION_HEADER)(pINH + 1);
		if (!pISH)
			return false;

		auto dwOldProtect = 0UL;
		SIZE_T size = sizeof(LPVOID);
		FVirtualProtectEx(NtCurrentProcess, (LPVOID)pISH, &size, PAGE_READWRITE, &dwOldProtect);

		pISH[0].VirtualAddress = reinterpret_cast<DWORD_PTR>(s_pGuardMem);

		FVirtualProtectEx(NtCurrentProcess, (LPVOID)pISH, &size, dwOldProtect, &dwOldProtect);

		return true;
	}

	bool IsDumpTriggered()
	{
		if (!s_pGuardMem)
		{
			LOG("Null guard ptr!");
			return true;
		}

		PSAPI_WORKING_SET_EX_INFORMATION pworkingSetExInformation = { s_pGuardMem, NULL };

		auto ntStatus = FNtQueryVirtualMemory(NtCurrentProcess, NULL, MemoryWorkingSetExInformation, &pworkingSetExInformation, sizeof(pworkingSetExInformation), NULL);
		if (NT_SUCCESS(ntStatus))
		{
			if (pworkingSetExInformation.VirtualAttributes.Valid)
				return true;
		}

		return false;
	}
};