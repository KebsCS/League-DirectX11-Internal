#pragma once

#include <Windows.h>
#include <atomic>

enum HookType
{
	HOOKTYPE_FORWARD_OVERWRITE,
	HOOKTYPE_FORWARD_CHAIN,
	HOOKTYPE_REVERSE_CHAIN
};

class FuncHook
{
private:
	FARPROC pCallable; // new function that we call
	uintptr_t pFunc; // original function
	uintptr_t pHook; // our hook
	std::atomic_bool bEnabled; // is hook enabled
	std::atomic_bool bStarted; // was hook enabled at least once
	HookType type;
	uint8_t arrOriginal[5]; // stores bytes of the original function
	uint8_t arrReHook[5]; // stores bytes of changed function for restoring the hook

	void rehook32(intptr_t offset);

public:
	FuncHook(HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: pFunc(NULL), pHook(NULL), bEnabled(false), bStarted(false), type(type)
	{
	}

	FuncHook(uintptr_t pFuncess, uintptr_t pHookess, HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: pFunc(pFuncess), pHook(pHookess), bEnabled(false), bStarted(false),
		type(type)
	{
	}

	FuncHook(void* pFuncess, void* pHookess, HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: FuncHook((uintptr_t)pFuncess, (uintptr_t)pHookess, type)
	{
	}

	FuncHook& operator=(const FuncHook& lhs)
	{
		this->pCallable = lhs.pCallable;
		this->pFunc = lhs.pFunc;
		this->pHook = lhs.pHook;
		this->bEnabled = lhs.bEnabled.load();
		this->bStarted = lhs.bStarted.load();
		this->type = lhs.type;
		memcpy(arrOriginal, lhs.arrOriginal, sizeof(arrOriginal));
		memcpy(arrReHook, lhs.arrReHook, sizeof(arrReHook));
		return *this;
	}

	~FuncHook()
	{
		if (bEnabled)
			unhook();
	}

	void hook();
	void unhook();

	template <typename Result, typename... Args>
	Result WINAPI Call(Args ... args);
};

class hook_guard
{
private:
	FuncHook& hook;
public:
	hook_guard(FuncHook& func_hook)
		: hook(func_hook)
	{
		hook.unhook();
	}

	~hook_guard()
	{
		hook.hook();
	}
};

template <typename Result, typename... Args>
Result WINAPI FuncHook::Call(Args ... args)
{
	hook_guard hg(*this);

	typedef Result(WINAPI* FunctionType)(Args ... args);

	return (Result)((FunctionType)pCallable)(args...);
}
