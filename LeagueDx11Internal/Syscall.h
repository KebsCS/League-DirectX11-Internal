#pragma once

#include <Windows.h>
#include <vector>

#include "Ntdefs.h"
#include "lazy_importer.h"
#include "crt.h"

inline HMODULE hNtdll = nullptr;

template <typename T = NTSTATUS>
class Syscall
{
private:
	char* executableMemory = nullptr;
	//HMODULE hNtdll = nullptr;

public:

	Syscall(std::vector<BYTE>index, LPCSTR func, uintptr_t offset = 0, std::vector<BYTE>ret = 0);
	~Syscall();

	template <typename... Args>
	T operator()(Args ... arg);

private:
	void Create(std::vector<BYTE>& index, LPCSTR& func, uintptr_t& offset, std::vector<BYTE>& ret);
};

template <typename T>
Syscall<T>::~Syscall()
{
	if (executableMemory)
		LI_FN(VirtualFreeEx)((HANDLE)-1, executableMemory, NULL, MEM_RELEASE);
}

template <typename T>
template <typename... Args>
T Syscall<T>::operator()(Args ... arg)
{
	if (!executableMemory)
		return T(0);

	// make a real function pointer to the memory containing opcodes, such that we can actually call it
	typedef T(__stdcall* mFunc)(Args ...);
	mFunc pFunc = mFunc(executableMemory);

	return pFunc(arg...);
}

template <typename T>
Syscall<T>::Syscall(std::vector<BYTE>index, LPCSTR func, uintptr_t offset, std::vector<BYTE>ret)
{
	Create(index, func, offset, ret);
}

template <typename T>
void Syscall<T>::Create(std::vector<BYTE>& index, LPCSTR& func, uintptr_t& offset, std::vector<BYTE>& ret)
{
	if (!hNtdll)
	{
		auto skNtdll = skCrypt("ntdll.dll");
		hNtdll = reinterpret_cast<HMODULE>(GetModuleBase(std::string(skNtdll).c_str()));
		skNtdll.clear();
	}

#ifdef _WIN64 // idk shellcode is fine but doesnt work

	if (index.size() > 4)
		return;

	byte shellcode[]
	{
		0x4C, 0x8B, 0xD1,					// mov r10, rcx
		0xB8, 0x00, 0x00, 0x00, 0x00,		// mov eax, function
		0x0F, 0x05,					        // syscall
		0xC3								// ret
	};

	for (size_t i = 0; i < index.size(); i++)
	{
		*(byte*)(shellcode + 4 + i) = index[i];
	}

	// allocate executable memory for opcodes
	executableMemory = static_cast<char*>(VirtualAlloc(nullptr, sizeof(shellcode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));

	if (!executableMemory)
		return;

	crt::memcpy(executableMemory, shellcode, sizeof(shellcode));

#elif _WIN32

	if (index.size() > 4 || ret.size() > 2)
		return;

	BYTE shellcode[] =
	{
		0xB8, 0x00, 0x00, 0x00, 0x00,		// mov eax, index
		0xBA, 0x00, 0x00, 0x00, 0x00,		// mov edx, function // ntdll.RtlInterlockedCompareExchange64+170
		0xFF, 0xD2,							// call edx
		0xC2, 0x00, 0x00					// ret
	};

	for (size_t i = 0; i < index.size(); i++)
	{
		*static_cast<BYTE*>(shellcode + 1 + i) = index[i];
	}

	for (size_t i = 0; i < ret.size(); i++)
	{
		*static_cast<BYTE*>(shellcode + 13 + i) = ret[i];
	}

	*(uintptr_t*)(shellcode + 6) = static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(GetProcedureAddress(hNtdll, func)) + offset);

	// allocate executable memory for opcodes
	executableMemory = static_cast<char*>(NtAllocateVirtualMemoryFunc((HANDLE)-1, nullptr, sizeof(shellcode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));

	if (!executableMemory)
		return;

	// move opcodes over from stack to executable memory
	crt::memcpy(executableMemory, shellcode, sizeof(shellcode));

#endif
}
