#include "VMTHook.h"
#include "crt.h"

bool VMTHook::Init(void* original)
{
	if (original)
	{
		originalVMT = reinterpret_cast<uintptr_t**>(original);
		copiedVMT = *originalVMT;

		while (reinterpret_cast<void*>(copiedVMT[iMethods]))
		{
			iMethods++;
		}

		if (iMethods > 0)
		{
			ourVMT = std::make_unique<std::uintptr_t[]>(iMethods);
			crt::memcpy(ourVMT.get(), copiedVMT, iMethods * sizeof(std::uintptr_t));
		}
	}
	return ourVMT != nullptr;
}

uintptr_t VMTHook::Hook(void* original, const size_t& index, const uintptr_t& function)
{
	if (!Init(original))
		return false;

	return HookIndex(index, function);
}

uintptr_t VMTHook::HookIndex(const size_t& index, const uintptr_t& function)
{
	if (ourVMT && index < iMethods)
	{
		ourVMT[index] = function;

		*originalVMT = ourVMT.get();
		bEnabled = true;

		return copiedVMT[index];
	}
	return 0;
}

bool VMTHook::UnHook()
{
	if (bEnabled)
	{
		*originalVMT = copiedVMT;
		bEnabled = false;
		return true;
	}

	return false;
}