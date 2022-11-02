#pragma once

#include "Includes.h"

class LowLevelHooks
{
private:
	HHOOK keyboardHook = NULL;
	HHOOK mouseHook = NULL;

	static LRESULT WINAPI LolLevelKeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		KBDLLHOOKSTRUCT* pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (nCode >= 0)
		{
			if ((pKeyboard->flags & LLKHF_INJECTED) != 0)
				pKeyboard->flags &= ~LLKHF_INJECTED;

			if ((pKeyboard->flags & LLKHF_LOWER_IL_INJECTED) != 0)
				pKeyboard->flags &= ~LLKHF_LOWER_IL_INJECTED;
		}

		return LI_FN(CallNextHookEx).get()(0, nCode, wParam, reinterpret_cast<LPARAM>(pKeyboard));
	}

	static LRESULT WINAPI LolLevelMouseHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSLLHOOKSTRUCT* pMouse = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

		if (nCode >= 0)
		{
			if ((pMouse->flags & LLMHF_INJECTED) != 0)
				pMouse->flags &= ~LLMHF_INJECTED;

			if ((pMouse->flags & LLMHF_LOWER_IL_INJECTED) != 0)
				pMouse->flags &= ~LLMHF_LOWER_IL_INJECTED;
		}

		return LI_FN(CallNextHookEx).get()(0, nCode, wParam, reinterpret_cast<LPARAM>(pMouse));
	}

public:

	bool Init()
	{
		keyboardHook = LI_FN(SetWindowsHookExW).get()(WH_KEYBOARD_LL, LolLevelKeyboardHook, NULL, 0);
		if (keyboardHook == NULL)
		{
			LOG("Low level keyboard hook failed");
			return false;
		}
		mouseHook = LI_FN(SetWindowsHookExW).get()(WH_MOUSE_LL, LolLevelMouseHook, NULL, 0);
		if (mouseHook == NULL)
		{
			LOG("Low level mouse hook failed");
			return false;
		}

		LOG("Hooked low level hooks");
		return true;
	}

	bool Release()
	{
		if (keyboardHook != NULL)
		{
			if (LI_FN(UnhookWindowsHookEx)(mouseHook) == false)
			{
				LOG("Failed to release low level keyboard hook");
				return false;
			}
		}
		if (mouseHook != NULL)
		{
			if (LI_FN(UnhookWindowsHookEx)(mouseHook) == false)
			{
				LOG("Failed to release low level mouse hook");
				return false;
			}
		}
		LOG("Unhooked low level hooks");
		return true;
	}
};