#include "Ntfuncs.h"
#include "FuncHook.h"

#include "crt.h"

#define JMP_32_SIZE				5
#define X86_NOP					0x90
#define X86_JMP					0xE9
#define X86_JMP_NEG_5			0xF9EB

class protect_guard
{
private:
	DWORD protect_val;
	SIZE_T size;
	LPVOID addr;
public:
	inline protect_guard(uintptr_t address, SIZE_T size)
		: addr(LPVOID(address)), size(size)
	{
		FVirtualProtectEx(NtCurrentProcess, addr, &size, PAGE_EXECUTE_READWRITE, &protect_val);
	}
	inline ~protect_guard()
	{
		FVirtualProtectEx(NtCurrentProcess, addr, &size, protect_val, &protect_val);
	}
};

void FuncHook::ReHook32(intptr_t offset)
{
	protect_guard guard(pFunc - JMP_32_SIZE, JMP_32_SIZE * 2);

	if (!bEnabled)
	{
		crt::memcpy(arrOriginal, LPVOID(pFunc), JMP_32_SIZE);
	}

	if (!bStarted)
	{
		uint8_t* p = (uint8_t*)pFunc - JMP_32_SIZE;
		size_t nop_count = 0;

		for (size_t i = 0; i < JMP_32_SIZE; ++i)
		{
			if (p[i] == X86_NOP)
			{
				nop_count++;
			}
		}

		if (nop_count == JMP_32_SIZE && p[5] == 0x8B && p[6] == 0xFF)
		{
			// hook_reverse_new
			pCallable = (FARPROC)(pFunc + 2);
			type = HOOKTYPE_REVERSE_CHAIN;
			bEnabled = true;

			p[0] = X86_JMP;
			*((uint32_t*)&p[1]) = (uint32_t)(pHook - pFunc);
			*((uint16_t*)&p[5]) = X86_JMP_NEG_5;
		}
		else if (p[0] == X86_JMP && *(uint16_t*)&p[5] == X86_JMP_NEG_5)
		{
			// hook_reverse_chain
			if (type == HOOKTYPE_FORWARD_OVERWRITE)
			{
				pCallable = (FARPROC)(pFunc + *((int32_t*)&p[1]));
				type = HOOKTYPE_REVERSE_CHAIN;
				bEnabled = true;

				*((uint32_t*)&p[1]) = (uint32_t)(pHook - pFunc);
			}
		}
		else if (p[5] == X86_JMP)
		{
			// hook_forward_chain
			int32_t cur_offset = *(int32_t*)&p[6];

			if (type == HOOKTYPE_FORWARD_OVERWRITE)
			{
				pCallable = (FARPROC)(pFunc + JMP_32_SIZE + cur_offset);
				type = HOOKTYPE_FORWARD_CHAIN;
				bEnabled = true;

				*((int32_t*)&p[6]) = (int32_t)offset;
			}
		}
		else
		{
			type = HOOKTYPE_FORWARD_OVERWRITE;
		}

		bStarted = true;
	}
	if (type == HOOKTYPE_FORWARD_OVERWRITE)
	{
		// hook_forward_overwrite

		uint8_t* ptr = (uint8_t*)pFunc;

		pCallable = (FARPROC)pFunc;
		type = HOOKTYPE_FORWARD_OVERWRITE;
		bEnabled = true;

		*(ptr++) = X86_JMP;
		*((int32_t*)ptr) = (int32_t)offset;
	}
}

void FuncHook::Hook()
{
	if (bEnabled)
		return;

	if (!pFunc || !pHook)
		return;

	if (bStarted)
	{
		uintptr_t addr;
		size_t size;

		if (type == HOOKTYPE_REVERSE_CHAIN)
		{
			addr = pFunc - JMP_32_SIZE;
			size = JMP_32_SIZE;
		}
		else
		{
			addr = pFunc;
			size = JMP_32_SIZE;
		}

		protect_guard guard(addr, size);
		crt::memcpy((void*)addr, arrReHook, size);
		bEnabled = true;
		return;
	}

	intptr_t offset = pHook - pFunc - JMP_32_SIZE;

	ReHook32(offset);
}

void FuncHook::UnHook()
{
	if (!bEnabled)
		return;

	uintptr_t addr;
	size_t size;

	if (type == HOOKTYPE_REVERSE_CHAIN)
	{
		size = JMP_32_SIZE;
		addr = (pFunc - JMP_32_SIZE);
	}
	else
	{
		size = JMP_32_SIZE;
		addr = pFunc;
	}

	protect_guard guard(addr, size);

	crt::memcpy(arrReHook, (void*)addr, size);

	// restore original bytes
	if (type == HOOKTYPE_FORWARD_OVERWRITE)
		crt::memcpy((void*)pFunc, arrOriginal, size);

	bEnabled = false;
}