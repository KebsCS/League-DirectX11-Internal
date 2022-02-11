#pragma once

#include "Includes.h"
#include "LeagueFuncs.h"
#include "GameObject.h"

class Prediction
{
private:

public:

	static Vector3 PosAfterTime(GameObject* obj, const float& delay)
	{
		AiManager* ai = obj->GetAiManager();
		const float ms = obj->GetStatTotal(StatType::MoveSpeed);
		const std::vector<Vector3>waypoints = ai->GetFuturePoints();
		const auto waypointsSize = waypoints.size();

		if (waypointsSize <= 1 || !ai->isMoving)
			return waypoints.front();

		float distance = ms * delay;
		for (auto i = 1; i < waypointsSize; i++)
		{
			const float waydistance = waypoints[i - 1].Distance(waypoints[i]);
			if (waydistance >= distance)
			{
				return waypoints[i - 1].Extend(waypoints[i], distance);
			}
			if (i == waypointsSize - 1)
				return waypoints[i];
			distance = distance - waydistance;
		}
		return waypoints.front();
	}

	struct PredOutput
	{
		Vector3 pos = Vector3(0, 0, 0);
		int hitChance = 0;
	};

	static PredOutput SkillInstant(GameObject* local, GameObject* target, float range, float radius, float castTime, int collision)
	{
		if (!target || !local)
			return {};

		const Vector3 localPos = local->GetAiManager()->serverPos;
		const Vector3 targetPos = local->GetAiManager()->serverPos;

		PredOutput pred;

		if (localPos.Distance(targetPos) > (range + 300.f))
			return pred;

		//todo add dashing, buffs, check when auto attacking, collisions

		Vector3 predictionPos = PosAfterTime(target, castTime);

		if (localPos.Distance(predictionPos) > range)
			return pred;

		pred.pos = predictionPos;
		pred.hitChance = 5;

		return pred;
	}
};