#pragma once

#include <Windows.h>

#include "Offsets.h"
#include "AiManager.h"
#include "Ntdefs.h"

class GameObject
{
public:
	char pad_0000[4]; //0x0000
	char pad_0004[28]; //0x0004
	uint32_t index; //0x0020
	char pad_0024[40]; //0x0024
	uint16_t N00000068; //0x004C
	char pad_004E[30]; //0x004E
	LolString name; //0x006C
	char pad_0084[72]; //0x0084
	uint32_t networkId; //0x00CC
	char pad_00D0[292]; //0x00D0
	Vector3 position; //0x01F4
	char pad_0200[180]; //0x0200
	float mana; //0x02B4
	char pad_02B8[12]; //0x02B8
	float maxMana; //0x02C4
	char pad_02C8[2796]; //0x02C8
	float health; //0x0DB4
	char pad_0DB8[12]; //0x0DB8
	float maxhealth; //0x0DC4
	char pad_0DC8[164]; //0x0DC8
	class N0000300B* Funcs; //0x0E6C
	char pad_0E70[532]; //0x0E70

	AiManager* GetAiManager() // GetPathController
	{
		return VFuncCall<AiManager*>(this, oVFunc::AiManager);
		/*int uVar2;
		int puVar3;
		int uStack4;
		int num1 = oAiManager - 4;
		uStack4 = *reinterpret_cast<int*>(this + (num1 + 8) + (*reinterpret_cast<byte*>(this + (num1 + 4))) * 4);
		puVar3 = (DWORD)this + num1;
		uVar2 = *reinterpret_cast<int*>(puVar3);
		uStack4 ^= ~uVar2;
		dwAiManager = *reinterpret_cast<DWORD*>(uStack4 + 8);

		return (AiManager*)(dwAiManager);*/
	}

	bool IsAlive()
	{
		return VFuncCall<bool>(this, oVFunc::IsAlive);
	}

	float GetBoundingRadius()
	{
		return VFuncCall<float>(this, oVFunc::BoundingRadius);
	}

	int IsFunc(ObjectType type)
	{
		if (!this)
			return 0;

		int uVar2;
		int puVar3;
		int uStack4;

		uStack4 = *reinterpret_cast<int*>((DWORD)this + 0x5c + (*reinterpret_cast<byte*>((DWORD)this + 0x58)) * 4);
		puVar3 = (DWORD)this + 0x54;
		uVar2 = *reinterpret_cast<int*>(puVar3);
		uStack4 ^= ~uVar2;

		return  (((int)type & uStack4) != 0);
	}
};