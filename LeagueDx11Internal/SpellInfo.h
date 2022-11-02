#pragma once

#include "Definitions.h"
#include "SpellData.h"

class SpellInfo
{
public:
	char pad_0000[24]; //0x0000
	LolString name; //0x0018
	char pad_0030[20]; //0x0030
	SpellData* spellData; //0x0044
	char pad_0048[76]; //0x0048
};