#pragma once

#include "Vector3.h"

class AiManager
{
public:
	char pad_0000[16]; //0x0000
	Vector3 targetPos; //0x0010
	char pad_001C[416]; //0x001C
	float movementSpeed; //0x01BC
	bool isMoving; //0x01C0
	char pad_01C1[3]; //0x01C1
	int16_t passedWaypoints; //0x01C4
	char pad_01C6[6]; //0x01C6
	Vector3 navBegin; //0x01CC
	char pad_01D8[32]; //0x01D8
	float dashSpeed; //0x01F8
	char pad_01FC[24]; //0x01FC
	bool isDashing; //0x0214
	char pad_0215[15]; //0x0215
	Vector3 navEnd; //0x0224
	char pad_0230[180]; //0x0230
	Vector3 serverPos; //0x02E4
	Vector3 velocity; //0x02F0
	char pad_02FC[1372]; //0x02FC
};