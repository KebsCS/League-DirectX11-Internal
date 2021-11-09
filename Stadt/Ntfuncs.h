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

// todo
BOOL FVirtualProtectEx(HANDLE hProcess, PVOID lpBaseAddress, PSIZE_T dwSize, ULONG flNewProtect, PULONG lpflOldPRotect);
typedef BOOL(WINAPI* tWriteProcessMemory)(
	HANDLE  hProcess,
	LPVOID  lpBaseAddress,
	LPCVOID lpBuffer,
	SIZE_T  nSize,
	SIZE_T* lpNumberOfBytesWritten
	);

//https://github.com/jjzhang166/WindowsUtil2/blob/0e8eae92d850db173620e60e012f688381b229f0/Process/WriteProcessMemory.cpp
// todo
BOOL FWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);

//
//HMODULE FLoadLibraryA(LPCSTR lpLibName)
//{
//	typedef HMODULE(WINAPI* tLoadLibraryA)(IN LPCSTR);
//	tLoadLibraryA pLoadLibraryA = NULL;
//
//	if (!pLoadLibraryA)
//	{
//		constexpr const char NtApi[] = { 'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y','A','\0' };
//
//		pLoadLibraryA = (tLoadLibraryA)(GetProcedureAddress(_getKernel32Handle(), NtApi));
//	}
//
//	return static_cast<HMODULE>(pLoadLibraryA(lpLibName));
//}

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