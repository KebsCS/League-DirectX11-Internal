#pragma once

#include <vector>

#include "Vector3.h"

#pragma warning (disable: 4409) //warning C4409: illegal instruction size

class NavigationPath
{
public:
	int32_t passedWaypoints; //0x0000
	char pad_0004[4]; //0x0004
	Vector3 startPos; //0x0008
	Vector3 endPos; //0x0014
	Vector3* navBegin; //0x0020
	Vector3* navEnd; //0x0024
	char pad[28]; // 0x0040
};

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

	std::vector<Vector3>GetFuturePoints()
	{
		const std::vector<Vector3>waypoints = GetWaypoints();
		const auto waypointsSize = waypoints.size();

		std::vector<Vector3>futurePoints;
		futurePoints.emplace_back(serverPos);
		if (waypointsSize == 0)
			return futurePoints;

		bool found = false;
		for (size_t i = 1; i < waypointsSize; i++)
		{
			if (!found)
			{
				// find between which points we are
				if (std::abs(serverPos.Distance(waypoints[i]) + serverPos.Distance(waypoints[i - 1]) - waypoints[i - 1].Distance(waypoints[i]))
					<= 20.f) // 20 tolerance, because the position isn't ideal and it can be moved by minions
				{
					futurePoints.emplace_back(waypoints[i]);
					found = true;
				}
			}
			else
			{
				futurePoints.emplace_back(waypoints[i]);
			}
		}

		return futurePoints;
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

	//std::vector<Vector3> CreatePath(Vector3 endPos, const bool smooth = true)
	//{
	//	uintptr_t create_path = RVA(oCreatePath);
	//	DWORD navigation_path = (DWORD)this + 0x1C4;
	//	reinterpret_cast<uint32_t* (__thiscall*)(AiManager*, DWORD, Vector3*)>(RVA(oCreatePath))((AiManager*)this, navigation_path, &endPos);

	//	if (smooth)
	//	{
	//		reinterpret_cast<int32_t(__thiscall*)(AiManager*, DWORD)>(RVA(0x0019FB90))((AiManager*)this, navigation_path);
	//	}

	//	std::vector<Vector3>waypoints;
	//	for (DWORD i = 0; i < (navEnd - navBegin); i += sizeof(Vector3))
	//	{
	//		const Vector3 point = *reinterpret_cast<Vector3*>(navBegin + i);
	//		if (!point.IsZero())
	//			waypoints.emplace_back(point);
	//	}
	//	return waypoints;
	//}

	std::vector<Vector3> BuildNavigationPath(Vector3 endPos, const bool& smooth = true)
	{
		Vector3 startPos = this->serverPos;
		bool unk1, unk2, unk3;
		unsigned maxNodes = 32; // if (!*((_BYTE *)Aimanager_ + 738)) maxNodes = 32 else maxNodes = 16
		bool unk4 = 0;// *reinterpret_cast<bool*>((DWORD)this + 0x304);
		DWORD* navMesh = *reinterpret_cast<DWORD**>((DWORD)this + 108 * 4);
		NavigationPath navPath = NavigationPath();

		//reinterpret_cast<uint32_t* (__thiscall*)
		//	(DWORD*, NavigationPath*, Vector3*, Vector3*, AiManager*, unsigned, bool*, bool*, bool*, unsigned, unsigned)>(RVA(oBuildNavigationPath))
		//	(navMesh, &navPath, &startPos, &endPos, (AiManager*)this, 0, &unk1, &unk2, &unk3, maxNodes, unk4);

		static DWORD fBuildNavigationPath = RVA(oBuildNavigationPath);
		AiManager* ai = (AiManager*)this;

		x86RetSpoof::invokeThiscall<bool>(std::uintptr_t(navMesh), fBuildNavigationPath, RVA(oSpoofGadget),
			&navPath, &startPos, &endPos, ai, 0, &unk1, &unk2, &unk3, maxNodes, unk4);

		if (smooth)
		{
			//reinterpret_cast<int32_t(__thiscall*)(AiManager*, NavigationPath*)>(RVA(oSmoothPath))((AiManager*)this, &navPath);
			static DWORD fSmoothPath = RVA(oSmoothPath);

			x86RetSpoof::invokeThiscall<int>(std::uintptr_t(ai), fSmoothPath, RVA(oSpoofGadget),
				&navPath);
		}

		std::vector<Vector3>waypoints;
		waypoints.reserve(((DWORD)navPath.navEnd - (DWORD)navPath.navBegin) / sizeof(Vector3));
		for (DWORD i = 0; i < ((DWORD)navPath.navEnd - (DWORD)navPath.navBegin); i += sizeof(Vector3))
		{
			const Vector3 point = *reinterpret_cast<Vector3*>((DWORD)navPath.navBegin + i);
			if (!point.IsZero())
				waypoints.emplace_back(point);
		}
		return waypoints;
	}
};