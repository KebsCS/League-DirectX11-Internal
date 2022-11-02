#pragma once

#include "x86RetSpoof.h"
#include "Offsets.h"


template <typename T>
T VFuncCall(void* object, const size_t& index)
{
	return x86RetSpoof::invokeThiscall<T>(reinterpret_cast<uintptr_t>(object), reinterpret_cast<uintptr_t>((*(void***)object)[index]), RVA(oSpoofGadget));
}