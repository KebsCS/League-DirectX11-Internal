#pragma once

#include "SpellInfo.h"

class SpellCastInfo
{
public:
	SpellInfo* spellInfo; //0x0000
	char pad_0004[120]; //0x0004
	Vector3 startPos; //0x007C
	Vector3 endPos; //0x0088
	Vector3 endPos2; //0x0094
	char pad_00A0[4024]; //0x00A0
}; //Size: 0x1058