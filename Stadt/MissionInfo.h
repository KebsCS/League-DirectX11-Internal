#pragma once

#include "Definitions.h"

class MissionInfo
{
public:
	char pad_0000[4]; //0x0000
	GameMap mapId; //0x0004
	char pad_0008[52]; //0x0008
	char* mapName; //0x003C
	char pad_0040[104]; //0x0040
	char* region; //0x00A8
	char pad_00AC[220]; //0x00AC
};