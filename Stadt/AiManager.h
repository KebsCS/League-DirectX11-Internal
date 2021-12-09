#pragma once

#include <vector>

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
	char pad_01C6[30]; //0x01C6
	DWORD navBegin; //0x01E4
	DWORD navEnd; //0x01E8
	char pad_01EC[12]; //0x01EC
	float dashSpeed; //0x01F8
	char pad_01FC[24]; //0x01FC
	bool isDashing; //0x0214
	char pad_0215[15]; //0x0215
	Vector3 destination; //0x0224
	char pad_0230[180]; //0x0230
	Vector3 serverPos; //0x02E4
	Vector3 velocity; //0x02F0
	char pad_02FC[1392]; //0x02FC

	std::vector<Vector3> GetWaypoints()
	{
		std::vector<Vector3> waypoints;
		if (navBegin != 0 && navEnd != 0)
		{
			waypoints.reserve((navEnd - navBegin) / sizeof(Vector3));

			for (DWORD i = 0; i < (navEnd - navBegin); i += sizeof(Vector3))
			{
				const Vector3 point = *reinterpret_cast<Vector3*>(navBegin + i);
				if (!point.IsZero())
					waypoints.emplace_back(point);
			}
		}

		return waypoints;
	}

	float GetPathLength()
	{
		std::vector points = GetWaypoints();
		if (points.size() == 0)
			return 0.f;

		float dist = 0.f;

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			dist += points[i].Distance(points[i + 1]);
		}
		return dist;
	}

	// todo
	/*std::vector<Vector3>GetFuturePoints()
	{
		std::vector<Vector3> waypoints;
		waypoints.reserve((navEnd - navBegin) / 0x0C);
		if (navBegin != 0 && navEnd != 0)
		{
			for (DWORD i = passedWaypoints-1; i < (navEnd - navBegin); i += 0x0C)
			{
				const Vector3 point = *reinterpret_cast<Vector3*>(navBegin + i);
				if (!point.IsZero())
					waypoints.emplace_back(point);
			}
		}

		return waypoints;
	}*/
};