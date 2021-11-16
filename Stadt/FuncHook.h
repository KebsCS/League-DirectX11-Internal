#pragma once

#include <Windows.h>
#include <atomic>
#include <mutex>

enum HookType
{
	HOOKTYPE_FORWARD_OVERWRITE,
	HOOKTYPE_FORWARD_CHAIN,
	HOOKTYPE_REVERSE_CHAIN
};

// todo clean this up
class FuncHook
{
private:
	FARPROC pCallable; // new function that we call
	uintptr_t pFunc; // original function
	uintptr_t pHook; // our hook
	std::atomic_bool bEnabled; // is hook enabled
	std::atomic_bool bStarted; // was hook enabled at least once
	HookType type;
	uint8_t arrOriginal[5] = { 0 }; // stores bytes of the original function
	uint8_t arrReHook[5] = { 0 }; // stores bytes of changed function for restoring the hook

	void ReHook32(intptr_t offset);

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

	FuncHook& operator=(const FuncHook& rhs)
	{
		this->pCallable = rhs.pCallable;
		this->pFunc = rhs.pFunc;
		this->pHook = rhs.pHook;
		this->bEnabled = rhs.bEnabled.load();
		this->bStarted = rhs.bStarted.load();
		this->type = rhs.type;
		memcpy(arrOriginal, rhs.arrOriginal, sizeof(arrOriginal));
		memcpy(arrReHook, rhs.arrReHook, sizeof(arrReHook));
		return *this;
	}

	~FuncHook()
	{
		if (bEnabled)
			UnHook();
	}

	void Hook();
	void UnHook();

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
		hook.UnHook();
	}

	~hook_guard()
	{
		hook.Hook();
	}
};

template <typename Result, typename... Args>
Result WINAPI FuncHook::Call(Args ... args)
{
	hook_guard hg(*this);

	typedef Result(WINAPI* FunctionType)(Args ... args);

	return (Result)((FunctionType)pCallable)(args...);
}
