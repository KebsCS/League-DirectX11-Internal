#pragma once

#include "Ntdefs.h"

HANDLE FOpenProcess(DWORD dwDesiredAccess, DWORD dwProcessId);

HANDLE FOpenThread(DWORD dwDesiredAccess, DWORD dwThreadId);

DWORD FSuspendThread(HANDLE hThread);

DWORD FResumeThread(HANDLE hThread);

BOOL FGetThreadContext(HANDLE hThread, LPCONTEXT lpContext);

BOOL FSetThreadContext(HANDLE hThread, CONTEXT* lpContext);

LPVOID FVirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType, DWORD flProtect);

SIZE_T FVirtualQueryEx(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);

SIZE_T FNtQueryVirtualMemory(HANDLE hProcess, PVOID BaseAddress, MEMORY_INFORMATION_CLASS  MemoryInformationClass,
	PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);

NTSTATUS FVirtualProtectEx(HANDLE hProcess, PVOID lpBaseAddress, PSIZE_T dwSize, ULONG flNewProtect, PULONG lpflOldPRotect);

typedef HANDLE(WINAPI* tCreateToolhelp32Snapshot)
(
	DWORD dwFlags,
	DWORD th32ProcessID
	);

HANDLE FCreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);

#define TH32CS_SNAPTHREAD   0x00000004

typedef struct tagTHREADENTRY32
{
	DWORD   dwSize;
	DWORD   cntUsage;
	DWORD   th32ThreadID;       // this thread
	DWORD   th32OwnerProcessID; // Process this thread is associated with
	LONG    tpBasePri;
	LONG    tpDeltaPri;
	DWORD   dwFlags;
} THREADENTRY32;
typedef THREADENTRY32* PTHREADENTRY32;
typedef THREADENTRY32* LPTHREADENTRY32;

typedef HANDLE(WINAPI* tThread32First)
(
	HANDLE hSnapshot,
	LPTHREADENTRY32 lpte
	);

HANDLE FThread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
typedef HANDLE(WINAPI* tThread32Next)
(
	HANDLE hSnapshot,
	LPTHREADENTRY32 lpte
	);

HANDLE FThread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte);

#define THREAD_ALL_ACCESS_VISTA         (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
	0xFFFF)

#define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
#define THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH 0x00000002
#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER 0x00000004
#define THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR 0x00000010
#define THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET 0x00000020
#define THREAD_CREATE_FLAGS_INITIAL_THREAD 0x00000080

NTSTATUS FNtCreateThreadEx(_Out_ PHANDLE ThreadHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ HANDLE ProcessHandle,
	_In_ PVOID StartRoutine,
	_In_opt_ PVOID Argument,
	_In_ ULONG CreateFlags,
	_In_opt_ ULONG_PTR ZeroBits,
	_In_opt_ SIZE_T StackSize,
	_In_opt_ SIZE_T MaximumStackSize,
	_In_opt_ PVOID AttributeList);

HMODULE LoadDll(LPCSTR lpFileName);

NTSTATUS FNtSetInformationProcess(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESS_INFORMATION_CLASS ProcessInformationClass,
	_In_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength
);

bool WINAPI FSetProcessMitigationPolicy(PROCESS_MITIGATION_POLICY MitigationPolicy, PVOID lpBuffer, SIZE_T dwLength);

BOOL WINAPI FSetProcessDEPPolicy();

BOOL WINAPI FNtUserSendInput(UINT cInputs, LPINPUT pInputs, int cbSize);