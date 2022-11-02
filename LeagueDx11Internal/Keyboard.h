#pragma once

#include "Includes.h"

class Keyboard
{
private:

	inline static std::map <WORD, DWORD>keys; // key,time
	inline static const DWORD delay = 30; // todo randomization

	inline static KEYBDINPUT kb = { 0 };
	inline static INPUT	input = { 0 };

public:

	// call in main loop
	static void Update(const DWORD& now)
	{
		for (auto it = keys.cbegin(); it != keys.cend();)
		{
			if (now - it->second > delay)
			{
				KeyUp(it->first);
				it = keys.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	static void KeyUp(const WORD& key)
	{
		ZeroMemory(&kb, sizeof(KEYBDINPUT));
		ZeroMemory(&input, sizeof(INPUT));

		kb.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		kb.wVk = 0;
		kb.wScan = key;
		kb.time = 0;
		kb.dwExtraInfo = 0;
		input.type = INPUT_KEYBOARD;
		input.ki = kb;
		FNtUserSendInput(1, &input, sizeof(input));
	}

	static void KeyDown(const WORD& key)
	{
		// we can't press the key again when it's down
		if (keys.contains(key))
			return;

		ZeroMemory(&kb, sizeof(KEYBDINPUT));
		ZeroMemory(&input, sizeof(INPUT));

		kb.dwFlags = KEYEVENTF_SCANCODE;
		kb.wVk = 0;
		kb.wScan = key;
		kb.time = 0;
		kb.dwExtraInfo = 0;
		input.type = INPUT_KEYBOARD;
		input.ki = kb;
		FNtUserSendInput(1, &input, sizeof(input));

		keys[key] = FGetTickCount();
	}
};
