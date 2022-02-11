#pragma once

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
	typedef void(__thiscall* tHandleSpellInput)(DWORD spellSlot, int slot, int state, float unk);
	static void HandleSpellInput(int slot, int state, float unk = 0)
	{
		static tHandleSpellInput fHandleSpellInput = (tHandleSpellInput)(Globals::dwBaseAddress + oHandleSpellInput);
		static DWORD dwSpellSlot = *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(RVA(oHudInstance)) + 52);

		// 1 - NormalCastSpell
		// 2 - QuickCastSpell
		// 3 - SelfCastSpell
		// 4 - QuickCastWithIndicatorSpell
		// 5 - QuickCastOrSelfCastSpell
		// 6 - QuickCastWithIndicatorOrSelfCastSpell
		// QuickbindCastSpell
		return fHandleSpellInput(dwSpellSlot, slot, state, unk);
	}

public:

	static void QuickCastSpell(Vector3 pos, SpellSlotID slot)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (pos2D.IsZero())
			return;

		HookedMouse.pos = pos2D;
		HookedMouse.enabled = true;
		LOG("pre cast");
		HandleSpellInput(static_cast<int>(slot), 2, 0);
		//HookedMouse.enabled = false;
		LOG("after cast");

		//HookedMouse.enabled = false;
	}
};