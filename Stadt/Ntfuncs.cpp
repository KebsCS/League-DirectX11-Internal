#pragma once

#include "Ntfuncs.h"
#include "Syscall.h"

//constexpr const char szRtlInterlockedCompareExchange64[] = { 'R', 't', 'l', 'I', 'n', 't', 'e', 'r', 'l', 'o', 'c','k','e','d','C','o','m','p','a','r','e','E','x','c','h','a','n','g','e','6','4','\0' };
std::string szRtlInterlockedCompareExchange64 = XorStr("RtlInterlockedCompareExchange64");
LPCSTR RtlInterlockedCompareExchange64 = szRtlInterlockedCompareExchange64.c_str();// szRtlInterlockedCompareExchange64;

HANDLE FOpenProcess(DWORD dwDesiredAccess, DWORD dwProcessId)
{
	HANDLE hProcess = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	CLIENT_ID clientId = { 0 };
	clientId.UniqueProcess = UlongToHandle(dwProcessId);
	InitializeObjectAttributes(&objectAttributes, NULL, NULL, NULL, NULL);

	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x26 }, RtlInterlockedCompareExchange64, 0x170, { 0x10 })(
		&hProcess, dwDesiredAccess, &objectAttributes, &clientId);

	SetLastError(ntStatus);
	return hProcess;
}

HANDLE FOpenThread(DWORD dwDesiredAccess, DWORD dwThreadId)
{
	HANDLE hThread = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	CLIENT_ID clientId = { 0 };
	clientId.UniqueThread = UlongToHandle(dwThreadId);
	InitializeObjectAttributes(&objectAttributes, NULL, NULL, NULL, NULL);

	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x2E,0x01 }, RtlInterlockedCompareExchange64, 0x170, { 0x10 })
		(&hThread, dwDesiredAccess, &objectAttributes, &clientId);

	SetLastError(ntStatus);
	return hThread;
}

DWORD FSuspendThread(HANDLE hThread)
{
	ULONG ulPreviousSuspendCount = NULL;
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0xBC,0x01,0x07 }, RtlInterlockedCompareExchange64, 0x170, { 0x08 })
		(hThread, &ulPreviousSuspendCount);
	return ulPreviousSuspendCount;
}

DWORD FResumeThread(HANDLE hThread)
{
	ULONG ulPreviousSuspendCount = NULL;
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x52,0x00,0x07 }, RtlInterlockedCompareExchange64, 0x170, { 0x08 })
		(hThread, &ulPreviousSuspendCount);
	return ulPreviousSuspendCount;
}

BOOL FGetThreadContext(HANDLE hThread, LPCONTEXT lpContext)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0xF2 }, RtlInterlockedCompareExchange64, 0x170, { 0x08 })
		(hThread, lpContext);
	if ((signed int)ntStatus >= 0)
		return 1;
	return 0;
}

BOOL FSetThreadContext(HANDLE hThread, CONTEXT* lpContext)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x8B,0x01 }, RtlInterlockedCompareExchange64, 0x170, { 0x08 })
		(hThread, lpContext);
	if ((signed int)ntStatus >= 0)
		return 1;
	return 0;
}

LPVOID FVirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType, DWORD flProtect)
{
	void* v10 = lpAddress;
	SIZE_T v11 = dwSize;
	DWORD nndPreferred = 0xFFFFFFFF;
	DWORD v6 = flAllocationType & 0xFFFFFFC0;
	if (nndPreferred != -1)
		v6 |= nndPreferred + 1;
	DWORD v7 = flProtect;
	DWORD v8 = v6;

	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x18 }, RtlInterlockedCompareExchange64, 0x170, { 0x18 })
		(hProcess, &v10, 0, &v11, v8, v7);
	if ((signed int)ntStatus >= 0)
		return v10;
	return 0;
}

SIZE_T FVirtualQueryEx(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength)
{
	ULONG ReturnLength;
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x50 }, RtlInterlockedCompareExchange64, 0x170, { 0x14 })
		(hProcess, lpAddress, lpBuffer, MemoryBasicInformation, dwLength, &ReturnLength);
	return ReturnLength;
}

// todo
BOOL FVirtualProtectEx(HANDLE hProcess, PVOID lpBaseAddress, PSIZE_T dwSize, ULONG flNewProtect, PULONG lpflOldPRotect)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x50 }, RtlInterlockedCompareExchange64, 0x170, { 0x14 })
		(hProcess, &lpBaseAddress, dwSize, flNewProtect, lpflOldPRotect);
	return 1;
}

tWriteProcessMemory WPM = NULL;

//https://github.com/jjzhang166/WindowsUtil2/blob/0e8eae92d850db173620e60e012f688381b229f0/Process/WriteProcessMemory.cpp
// todo
BOOL FWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
{
	//NtProtectVirtualMemory(hProcess,lpBaseAddress,);
	//SIZE_T queryLength = 0;
	//NTSTATUS ntStatus = NtQueryVirtualMemory(hProcess, lpBaseAddress, (MEMORY_INFORMATION_CLASS)0, 0, 0, &queryLength);

	//PVOID lpPageStartAddress = lpBaseAddress;
	//SIZE_T lpPageSize = nSize;
	//SIZE_T oldProtect = 0;

	//NTSTATUS status = NtProtectVirtualMemory(hProcess, &lpPageStartAddress, &lpPageSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	//if (!NT_SUCCESS(status))
	//{
	//	std::cerr << "NtProtectVirtualMemory failed " << status;
	//	exit(0);
	//}
	//VirtualProtectEx(hProcess, lpBaseAddress, lpPageSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	//NtWriteVirtualMemory(hProcess, lpBaseAddress, &lpBuffer, nSize, lpNumberOfBytesWritten);

	//VirtualProtectEx(hProcess, lpBaseAddress, lpPageSize, oldProtect, &oldProtect);
	////NtProtectVirtualMemory(hProcess, &lpPageStartAddress, &lpPageSize, oldProtect, &oldProtect);
	if (!WPM)
	{
		//constexpr const char szWPM[] = { 'W', 'r', 'i', 't', 'e', 'P', 'r', 'o', 'c', 'e', 's','s','M','e','m','o','r','y','\0' };
		std::string szWPM = XorStr("WriteProcessMemory");
		WPM = (tWriteProcessMemory)(GetProcedureAddress(GetKernel32Handle(), szWPM.c_str()));
	}

	WPM(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);

	//WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);

	return 1; // todo
}

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

tCreateToolhelp32Snapshot pCreateToolhelp32Snapshot = NULL;

HANDLE FCreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID)
{
	if (!pCreateToolhelp32Snapshot)
	{
		//constexpr const char szCreateToolhelp32Snapshot[] = { 'C', 'r', 'e', 'a', 't', 'e', 'T', 'o', 'o', 'l', 'h', 'e','l','p','3','2','S','n','a','p','s','h','o','t', '\0' };
		std::string szCreateToolhelp32Snapshot = XorStr("CreateToolhelp32Snapshot");
		pCreateToolhelp32Snapshot = (tCreateToolhelp32Snapshot)(GetProcedureAddress(GetKernel32Handle(), szCreateToolhelp32Snapshot.c_str()));
	}
	return pCreateToolhelp32Snapshot(dwFlags, th32ProcessID);
}

tThread32First pThread32First = NULL;

HANDLE FThread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte)
{
	if (!pThread32First)
	{
		//constexpr const char szThread32First[] = { 'T', 'h', 'r', 'e', 'a', 'd', '3', '2', 'F', 'i', 'r', 's','t','\0' };
		std::string szThread32First = XorStr("Thread32First");
		pThread32First = (tThread32First)(GetProcedureAddress(GetKernel32Handle(), szThread32First.c_str()));
	}
	return pThread32First(hSnapshot, lpte);
}

tThread32Next  pThread32Next = NULL;

HANDLE FThread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte)
{
	if (!pThread32Next)
	{
		//constexpr const char szThread32Next[] = { 'T', 'h', 'r', 'e', 'a', 'd', '3', '2', 'N', 'e', 'x', 't','\0' };
		std::string szThread32Next = XorStr("Thread32Next");
		pThread32Next = (tThread32Next)(GetProcedureAddress(GetKernel32Handle(), szThread32Next.c_str()));
	}
	return pThread32Next(hSnapshot, lpte);
}

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
	_In_opt_ PVOID AttributeList)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0xC1 }, RtlInterlockedCompareExchange64, 0x170, { 0x2C })
		(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, ZeroBits, StackSize, MaximumStackSize, AttributeList);
	return ntStatus;
}