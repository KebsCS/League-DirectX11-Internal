#pragma once

#include <Windows.h>

#include "Offsets.h"
#include "AiManager.h"
#include "Ntdefs.h"

class GameObject
{
public:

	AiManager* GetAiManager()
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

	float GetBoundingRadius()
	{
		return VFuncCall<float>(this, oVFunc::BoundingRadius);
	}
};