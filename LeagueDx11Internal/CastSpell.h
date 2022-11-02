#pragma once

#include <queue>

#include "Includes.h"
#include "LeagueFuncs.h"
#include "GameObject.h"

struct HookedMouseStruct
{
	POINT pos;
	bool enabled = false;
	std::mutex mutex;
} static HookedMouse;

class CastSpell
{
private:

	//sub_D396D0("evtSmartCastWithIndicatorAvatarSpell1", 37);
	//sub_122DB70((_DWORD*)v1 + 19, (int)&v68, (int)HandleSpellInputWrapper);
	//void __usercall HudSpellLogic::HandleSpellInput(long double a1@<st0>, HudSpellLogic *a2, int a3, int a4, int a5)
	//HudSpellLogic::HandleSpellInput(HudSpellLogic::SpellSlot,HudSpellLogic::SpellInput,UIEvent::KeyState)
	//typedef void(__thiscall* tHandleSpellInput)(DWORD spellSlot, int slot, int state, float unk);
	//static void HandleSpellInput(int slot, int state, float unk = 0)
	//{
	//	static tHandleSpellInput fHandleSpellInput = (tHandleSpellInput)(Globals::dwBaseAddress + oHandleSpellInput);
	//	static DWORD dwSpellSlot = *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(RVA(oHudInstance)) + 52);

	//	// 1 - NormalCastSpell
	//	// 2 - QuickCastSpell
	//	// 3 - SelfCastSpell
	//	// 4 - QuickCastWithIndicatorSpell
	//	// 5 - QuickCastOrSelfCastSpell
	//	// 6 - QuickCastWithIndicatorOrSelfCastSpell
	//	// QuickbindCastSpell
	//	return fHandleSpellInput(dwSpellSlot, slot, state, unk);
	//}

	static inline std::queue<INPUT>inputs;

#define SENDINPUTX(x) (x * 65536 / (1920)+1)
#define SENDINPUTY(y) (y * 65536 / (1080)+1)

public:

	static size_t InputsSize()
	{
		return inputs.size();
	}

	static void Update()
	{
		if (!inputs.empty())
		{
			INPUT input = inputs.front();
			FNtUserSendInput(1, &input, sizeof(INPUT));
			inputs.pop();
		}
	}

	static void Cast(const WORD& key, const Vector2& pos)
	{
		POINT curMouse{};
		bool getMouse = LI_FN(GetCursorPos).get()(&curMouse);
		if (!getMouse)
			return;

		INPUT input;

		// move mouse
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		input.mi.dx = SENDINPUTX(pos.x);
		input.mi.dy = SENDINPUTY(pos.y);
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		inputs.push(input);

		// key down
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
		input.ki.wVk = 0;
		input.ki.wScan = key;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		inputs.push(input);

		// key up
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		input.ki.wVk = 0;
		input.ki.wScan = key;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		inputs.push(input);

		// mouse back
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		input.mi.dx = SENDINPUTX(curMouse.x);
		input.mi.dy = SENDINPUTY(curMouse.y);
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		inputs.push(input);
	}

	//static void QuickCastSpell(Vector3 pos, SpellSlotID slot)
	//{
	//	Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
	//	if (pos2D.IsZero())
	//		return;

	//	HookedMouse.pos = pos2D;
	//	HookedMouse.enabled = true;
	//	LOG("pre cast");
	//	HandleSpellInput(static_cast<int>(slot), 2, 0);
	//	//HookedMouse.enabled = false;
	//	LOG("after cast");

	//	//HookedMouse.enabled = false;
	//}
};