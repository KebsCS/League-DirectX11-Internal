#pragma once

#include "Includes.h"
#include "LeagueFuncs.h"
#include "GameObject.h"

class IssueOrder
{
private:

	// state - idk? keydown keyup?
	// isAttack - turns curson red
	// isAttackCommand - idk?
	// x,y - 2d screen cords
	// attackOnly - can't move, only attack
	typedef bool(__thiscall* tTryRightClick)(DWORD hudInputLogic, int state, int isAttack, bool isAttackCommand, int x, int y, bool attackOnly);
	static int TryRightClick(int state, int isAttack, bool isAttackCommand, int x, int y, bool attackOnly)
	{
		static tTryRightClick fTryRightClick = (tTryRightClick)(Globals::dwBaseAddress + oTryRightClick);
		static DWORD dwHudInstance = *reinterpret_cast<DWORD*>(RVA(oHudInstance));
		static DWORD dwHudInputLoic = *reinterpret_cast<DWORD*>(dwHudInstance + 0x24);
		return fTryRightClick(dwHudInputLoic, state, isAttack, isAttackCommand, x, y, attackOnly);
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

	static void AttackMove(Vector3 pos)
	{
		Vector2 pos2D = LeagueFuncs::WorldToScreen(pos);
		if (!pos2D.IsZero())
		{
			TryRightClick(0, 1, false, pos2D.x, pos2D.y, 0);
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