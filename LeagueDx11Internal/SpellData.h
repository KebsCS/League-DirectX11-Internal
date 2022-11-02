#pragma once

class SpellData
{
public:
	char pad_0000[516]; //0x0000
	char* mAnimationName; //0x0204
	char pad_0208[128]; //0x0208
	float cooldownTime; //0x0288
	char pad_028C[328]; //0x028C
	float castRange; //0x03D4
	char pad_03D8[24]; //0x03D8
	float castRangeDisplayOverride; //0x03F0
	char pad_03F4[24]; //0x03F4
	float castRadius; //0x040C
	char pad_0410[284]; //0x0410
	float mana; //0x052C
	char pad_0530[548]; //0x0530
}; //Size: 0x0754
