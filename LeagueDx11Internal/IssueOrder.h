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
		DWORD now = FGetTickCount();
		if (now - lastTick < delay)
			return;
		lastTick = now;

		static DWORD fTryRightClick = RVA(oTryRightClick);
		static DWORD dwHudInstance = *reinterpret_cast<DWORD*>(RVA(oHudInstance));
		DWORD dwHudInputLoic = *reinterpret_cast<DWORD*>(dwHudInstance + 0x24);

		x86RetSpoof::invokeThiscall<bool>(dwHudInputLoic, fTryRightClick, RVA(oSpoofGadget),
			state, isAttack, isAttackCommand, x, y, attackOnly);
	}

public:

	static void Move(const Vector2& pos2D)
	{
		TryRightClick(0, 0, false, pos2D.x, pos2D.y, 0);
	}

	static void Move(const Vector3& pos)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (!pos2D.IsZero())
		{
			Move(pos2D);
		}
	}

	static void AttackMove(const Vector2& pos2D)
	{
		TryRightClick(0, 1, false, pos2D.x, pos2D.y, 0);
	}

	static void AttackMove(const Vector3& pos)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (!pos2D.IsZero())
		{
			AttackMove(pos2D);
		}
	}

	static void Attack(const GameObject* target)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(target->position);
		if (!pos2D.IsZero())
		{
			TryRightClick(0, 0, true, pos2D.x, pos2D.y, 1);
		}
	}
};