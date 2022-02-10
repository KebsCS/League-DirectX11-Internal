#pragma once

#include "Includes.h"
#include "LeagueFuncs.h"
#include "GameObject.h"

#pragma warning (disable: 4409) //warning C4409: illegal instruction size

class IssueOrder
{
private:

	inline static DWORD lastTick;
	const static DWORD delay = 26; // in milliseconds

	// todo, when holding right click the cursor doesnt appear,
	// test what parameter gets set for it

	// state - idk? keydown keyup?
	// isAttack - turns curson red
	// isAttackCommand - idk?
	// x,y - 2d screen cords
	// attackOnly - can't move, only attack
	//typedef bool(__thiscall* tTryRightClick)(DWORD hudInputLogic, int state, int isAttack, bool isAttackCommand, int x, int y, bool attackOnly);
	static void TryRightClick(int state, int isAttack, bool isAttackCommand, int x, int y, bool attackOnly)
	{
		DWORD now = LI_FN(GetTickCount).get()();
		if (now - lastTick < delay)
			return;
		lastTick = now;

		/*DWORD spoofAddress = RVA(oSpoofAddress);
		if (((*reinterpret_cast<DWORD*>(spoofAddress)) & 0xFF) != 0xC3)
		{
			LOG("Not C3");
			return;
		}*/

		//static tTryRightClick fTryRightClick = (tTryRightClick)(Globals::dwBaseAddress + oTryRightClick);
		static DWORD fTryRightClick = RVA(oTryRightClick);
		static DWORD dwHudInstance = *reinterpret_cast<DWORD*>(RVA(oHudInstance));
		DWORD dwHudInputLoic = *reinterpret_cast<DWORD*>(dwHudInstance + 0x24);

		std::array<std::uint8_t, 2> gadget = *(std::array<std::uint8_t, 2>*)RVA(oSpoofGadget);
		if (gadget.at(0) != 0xFF || gadget.at(1) != 0x23)
		{
			LOG("Wrong spoof gadget");
			return;
		}
		x86RetSpoof::invokeThiscall<void*>(dwHudInputLoic, fTryRightClick, std::uintptr_t(gadget.data()), state, isAttack, isAttackCommand, x, y, attackOnly);

		// set ecx when func is thiscall
		// and the parameters go backwards
		/*__asm
		{
			push retnHere
			mov ecx, dwHudInputLoic
			push attackOnly
			push y
			push x
			push isAttackCommand
			push isAttack
			push state
			push spoofAddress
			jmp fTryRightClick
			retnHere :
		}*/
	}

public:

	static void Move(Vector2 pos2D)
	{
		TryRightClick(0, 0, false, pos2D.x, pos2D.y, 0);
	}

	static void Move(Vector3 pos)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (!pos2D.IsZero())
		{
			Move(pos2D);
		}
	}

	static void AttackMove(Vector2 pos2D)
	{
		TryRightClick(0, 1, false, pos2D.x, pos2D.y, 0);
	}

	static void AttackMove(Vector3 pos)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (!pos2D.IsZero())
		{
			AttackMove(pos2D);
		}
	}

	static void Attack(GameObject* target)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(target->position);
		if (!pos2D.IsZero())
		{
			TryRightClick(0, 0, true, pos2D.x, pos2D.y, 1);
		}
	}
};