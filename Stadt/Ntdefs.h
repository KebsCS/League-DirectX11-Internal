#pragma once

#include <Windows.h>

#include "XorString.h"

#define NtCurrentProcess (HANDLE)0xFFFFFFFF

#if !defined(NTSTATUS)
typedef LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#ifndef NT_ERROR
#define NT_ERROR(Status) ((ULONG)(Status) >> 30 == 3)
#endif

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING, ** PPUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		struct
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	_ACTIVATION_CONTEXT* EntryPointActivationContext;
	PVOID PatchInformation;
	LIST_ENTRY ForwarderLinks;
	LIST_ENTRY ServiceTagLinks;
	LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;        // SECURITY_DESCRIPTOR
	PVOID SecurityQualityOfService;  // SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation,
	MemoryWorkingSetInformation,
	MemoryMappedFilenameInformation,
	MemoryRegionInformation,
	MemoryWorkingSetExInformation,
	MemorySharedCommitInformation
} MEMORY_INFORMATION_CLASS;

typedef union _PSAPI_WORKING_SET_EX_BLOCK {
	ULONG_PTR   Flags;
	struct {
		ULONG_PTR   Valid : 1;
		ULONG_PTR   ShareCount : 3;
		ULONG_PTR   Win32Protection : 11;
		ULONG_PTR   Shared : 1;
		ULONG_PTR   Node : 6;
		ULONG_PTR   Locked : 1;
		ULONG_PTR   LargePage : 1;
	};
} PSAPI_WORKING_SET_EX_BLOCK;
typedef PSAPI_WORKING_SET_EX_BLOCK* PPSAPI_WORKING_SET_EX_BLOCK;

typedef struct _PSAPI_WORKING_SET_EX_INFORMATION {
	PVOID                       VirtualAddress;
	PSAPI_WORKING_SET_EX_BLOCK  VirtualAttributes;
} PSAPI_WORKING_SET_EX_INFORMATION;
typedef PSAPI_WORKING_SET_EX_INFORMATION* PPSAPI_WORKING_SET_EX_INFORMATION;

// https://github.com/Visgean/Zeus/blob/translation/source/client/core.cpp#L175
HMODULE GetKernel32Handle(void);

//https://www.unknowncheats.me/wiki/C%2B%2B:Detecting_and_Removing_API_Hooks_using_C%2B%2B
FARPROC GetProcedureAddress(HANDLE hModule, LPCSTR pszProcName);

// https://github.com/apt69/RibeyeSpecial/blob/b56b225e8ae6507d514931f935445bd74b82de77/RibeyeBone/ImportHandler.cpp#L56
void* GetModuleBase(LPCSTR moduleName);

typedef NTSTATUS(NTAPI* tNtAllocateVirtualMemory)
(
	IN HANDLE ProcessHandle,
	IN OUT PVOID* BaseAddress,
	IN ULONG_PTR ZeroBits,
	IN OUT PSIZE_T RegionSize,
	IN ULONG AllocationType,
	IN ULONG Protect
	);

// VirtualAllocEx
LPVOID NtAllocateVirtualMemoryFunc(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType, DWORD flProtect);

template <typename Result, typename... Args>
Result WINAPI ModuleCall(LPCSTR module, LPCSTR procName, Args ... args)
{
	typedef Result(WINAPI* FunctionType)(Args ... args);

	void* base = nullptr;
	if (strcmp(module, XorStr("kernel32.dll")) == 0)
		base = GetKernel32Handle;
	else
		base = GetModuleBase(module);

	FARPROC proc = GetProcedureAddress(base, procName);
	if (proc == nullptr)
	{
		//throw Instalog::SystemFacades::Win32Exception::FromLastError();
	}

	return (Result)((FunctionType)proc)(args...);
}

uintptr_t GetCurrentPID();

template <typename T, typename ... Args>
T VFuncCall(void* object, size_t index, Args...args)
{
	return reinterpret_cast<T(__thiscall*)(void*, Args...args)>((*(void***)object)[index])(object, args...);
}