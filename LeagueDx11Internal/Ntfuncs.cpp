#pragma once

#include "Ntfuncs.h"
#include "Syscall.h"

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

	//SetLastError(ntStatus);
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

	//SetLastError(ntStatus);
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

SIZE_T FNtQueryVirtualMemory(HANDLE hProcess, PVOID BaseAddress, MEMORY_INFORMATION_CLASS  MemoryInformationClass,
	PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x50 }, RtlInterlockedCompareExchange64, 0x170, { 0x14 })
		(hProcess, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
	return ntStatus;
}

// todo, handle the args, but works fine anyway
NTSTATUS FVirtualProtectEx(HANDLE hProcess, PVOID lpBaseAddress, PSIZE_T dwSize, ULONG flNewProtect, PULONG lpflOldPRotect)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x50 }, RtlInterlockedCompareExchange64, 0x170, { 0x14 })
		(hProcess, &lpBaseAddress, dwSize, flNewProtect, lpflOldPRotect);
	return ntStatus;
}

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

typedef NTSTATUS(WINAPI* fLdrLoadDll) //LdrLoadDll function prototype
(
	IN PWCHAR PathToFile OPTIONAL,
	IN ULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName,
	OUT HMODULE* ModuleHandle
	);

/**************************************************************************
  *      RtlInitUnicodeString   (NTDLL.@)
  *
  * Initializes a buffered unicode string.
  *
  * RETURNS
  *  Nothing.
  *
  * NOTES
  *  Assigns source to target->Buffer. The length of source is assigned to
  *  target->Length and target->MaximumLength. If source is NULL the length
  *  of source is assumed to be 0.
  */
void WINAPI RtlInitUnicodeString(
	PUNICODE_STRING target, /* [I/O] Buffered unicode string to be initialized */
	PCWSTR source)          /* [I]   '\0' terminated unicode string used to initialize target */
{
	if ((target->Buffer = (PWSTR)source))
	{
		auto length = lstrlenW(source) * sizeof(WCHAR);
		if (length > 0xfffc)
			length = 0xfffc;
		target->Length = length;
		target->MaximumLength = target->Length + sizeof(WCHAR);
	}
	else target->Length = target->MaximumLength = 0;
}

fLdrLoadDll   _LdrLoadDll = NULL;

// LoadLibrary wrapper, unsafe
HMODULE LoadDll(LPCSTR lpFileName)
{
	if (!hNtdll)
	{
		auto skNtdll = skCrypt("ntdll.dll");
		hNtdll = reinterpret_cast<HMODULE>(GetModuleBase(std::string(skNtdll).c_str()));
		skNtdll.clear();
	}
	if (!_LdrLoadDll) // can be a syscall, todo
	{
		std::string szLdrLoadDll = XorStr("LdrLoadDll");
		_LdrLoadDll = (fLdrLoadDll)GetProcedureAddress(hNtdll, szLdrLoadDll.c_str());
	}

	wchar_t wFileName[100];
	LI_FN(MultiByteToWideChar)(CP_ACP, MB_PRECOMPOSED, lpFileName, -1, wFileName, 100);

	UNICODE_STRING usDllFile;
	RtlInitUnicodeString(&usDllFile, wFileName);

	HMODULE hModule = NULL;
	NTSTATUS result = _LdrLoadDll(NULL, NULL, &usDllFile, &hModule);

	return hModule;
}

NTSTATUS FNtSetInformationProcess(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_In_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength
)
{
	NTSTATUS ntStatus = Syscall<NTSTATUS>({ 0x1C }, RtlInterlockedCompareExchange64, 0x170, { 0x10 })
		(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength);
	return ntStatus;
}

bool WINAPI FSetProcessMitigationPolicy(PROCESS_MITIGATION_POLICY MitigationPolicy, PVOID lpBuffer, SIZE_T dwLength)
{
	char v3; // dl
	bool v4; // zf
	signed int v5; // eax
	NTSTATUS v6; // eax
	NTSTATUS v8; // ecx
	signed int v9; // [esp-4h] [ebp-10h]
	int v10; // [esp+4h] [ebp-8h]
	int ProcessInformation; // [esp+8h] [ebp-4h]

	switch (MitigationPolicy)
	{
	case 0:
		if (dwLength != 8 || *(DWORD*)lpBuffer & 0xFFFFFFFC)
			goto LABEL_21;
		if (*(BYTE*)lpBuffer & 1)
		{
			v3 = *(BYTE*)((DWORD)lpBuffer + 4);
			if (!v3)
				goto LABEL_21;
			v4 = (*(BYTE*)lpBuffer & 2) == 0;
			v5 = 1;
			ProcessInformation = 1;
			if (v4)
				goto LABEL_7;
			v9 = 5;
		}
		else
		{
			if (*(BYTE*)lpBuffer & 2)
			{
				v8 = -1073741776;
				goto LABEL_25;
			}
			v3 = *(BYTE*)((DWORD)lpBuffer + 4);
			v9 = 2;
		}
		v5 = v9;
		ProcessInformation = v9;
	LABEL_7:
		if (v3)
			ProcessInformation = v5 | 8;
		v6 = FNtSetInformationProcess((HANDLE)0xFFFFFFFF, ProcessExecuteFlags, &ProcessInformation, 4u);
		goto LABEL_10;
	case 1:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 1;
		goto LABEL_15;
	case 2:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 2;
		goto LABEL_15;
	case 3:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 3;
		goto LABEL_15;
	case 4:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 4;
		goto LABEL_15;
	case 6:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 6;
		goto LABEL_15;
	case 7:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 7;
		goto LABEL_15;
	case 8:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 8;
		goto LABEL_15;
	case 9:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 9;
		goto LABEL_15;
	case 10:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 10;
		goto LABEL_15;
	case 13:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 13;
		goto LABEL_15;
	case 14:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 14;
		goto LABEL_15;
	case 15:
		if (dwLength != 4)
			goto LABEL_21;
		v10 = 15;
	LABEL_15:
		ProcessInformation = *(DWORD*)lpBuffer;
		v6 = FNtSetInformationProcess((HANDLE)0xFFFFFFFF, (PROCESSINFOCLASS)(ProcessCookie | ProcessUserModeIOPL), &v10, 8u);
	LABEL_10:
		if (v6 >= 0)
			return 1;
		v8 = v6;
	LABEL_25:
		//sub_10111000(v8); // SetLastError
		return 0;
	default:
	LABEL_21:
		v8 = -1073741811;
		goto LABEL_25;
	}
}

BOOL WINAPI FSetProcessDEPPolicy() // PROCESS_DEP_ENABLE
{
	PROCESS_MITIGATION_DEP_POLICY policy = { 0 };
	policy.Enable = 1;
	policy.Permanent = TRUE;
	return FSetProcessMitigationPolicy(ProcessDEPPolicy, &policy, sizeof(policy));
}

// https://github.com/Hypercall/MouseController/blob/master/MouseController/MouseController.cpp
BOOL WINAPI FNtUserSendInput(UINT cInputs, LPINPUT pInputs, int cbSize)
{
	static LPVOID NtUserSendInput_Addr = NULL;
	static BYTE NtUserSendInput_Bytes[30];
	if (!NtUserSendInput_Addr)
	{
		auto skNtUserSendInput = skCrypt("NtUserSendInput");
		auto skwin32u = skCrypt("win32u.dll");
		auto skuser32 = skCrypt("user32.dll");

		// windows 8.1 / windows 10
		NtUserSendInput_Addr = GetProcedureAddress(GetModuleBase(std::string(skwin32u).c_str()), std::string(skNtUserSendInput).c_str());
		if (!NtUserSendInput_Addr)
		{
			NtUserSendInput_Addr = GetProcedureAddress(GetModuleBase(std::string(skuser32).c_str()), std::string(skNtUserSendInput).c_str());
			//if (!NtUserSendInput_Addr)
			//{
			//	// Windows 7 or lower detected
			//	NtUserSendInput_Addr = GetProcedureAddress(GetModuleBase("user32"), "SendInput");
			//	if (!NtUserSendInput_Addr)
			//		return FALSE;
			//}
		}
		skNtUserSendInput.clear();
		skwin32u.clear();
		skuser32.clear();
		crt::memcpy(NtUserSendInput_Bytes, NtUserSendInput_Addr, 30);
	}

	LPVOID NtUserSendInput_Spoof = FVirtualAllocEx(NtCurrentProcess, 0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//LPVOID NtUserSendInput_Spoof = VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE); // allocate space for syscall
	if (!NtUserSendInput_Spoof)
		return FALSE;
	crt::memcpy(NtUserSendInput_Spoof, NtUserSendInput_Bytes, 30); // copy syscall
	NTSTATUS Result = reinterpret_cast<NTSTATUS(NTAPI*)(UINT, LPINPUT, int)>(NtUserSendInput_Spoof)(cInputs, pInputs, cbSize); // calling spoofed function
	ZeroMemory(NtUserSendInput_Spoof, 0x1000); // clean address
	//FVirtualFree(NtUserSendInput_Spoof, 0, MEM_RELEASE); // free it
	LI_FN(VirtualFreeEx)((HANDLE)-1, NtUserSendInput_Spoof, NULL, MEM_RELEASE);
	return (Result > 0); // return the status
}