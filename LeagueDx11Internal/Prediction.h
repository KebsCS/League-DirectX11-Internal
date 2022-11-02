#pragma once

#include "Includes.h"
#include "LeagueFuncs.h"
#include "GameObject.h"

class Prediction
{
private:

public:

	// todo, calculate speed buffs, or dont offset by bounding radius, when can get speed up
	static Vector3 PosAfterTime(GameObject* obj, const float& delay, float missileWidth)
	{
		AiManager* ai = obj->GetAiManager();
		float ms = obj->GetStatTotal(StatType::MoveSpeed);
		if (ai->isDashing)
		{
			ms = ai->dashSpeed;
		}

		const std::vector<Vector3>waypoints = ai->GetFuturePoints();
		const size_t waypointsSize = waypoints.size();

		if (waypointsSize <= 1 || !ai->isMoving)
			return waypoints.front();

		// offset, because they wont get past the center of predicted position,
		// so we are essentialy just wasting the hitbox, unless they get speed up
		float distance = (ms * delay) - missileWidth;

		//todo offset, or not when dash ends prematurely, also check if blink to center hitbox
		for (size_t i = 1; i < waypointsSize; i++)
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

	//static Vector3 Preidct22(GameObject* local, GameObject* target, float range, float radius, float castTime, float speed, int collision)
	//{
	//	auto waypoints = target->GetAiManager()->GetFuturePoints();
	//	if (waypoints.size() <= 1)
	//		return waypoints.front();
	//
	//	Vector3 disp = waypoints[1] - waypoints[0]; // direction to next point
	//	float distance = disp.Length();
	//	Vector3 velocity = disp * (target->GetStatTotal(StatType::MoveSpeed) / distance);

	//	Vector3 dispMissile = target->GetAiManager()->serverPos - local->GetAiManager()->serverPos;
	//	float distanceMissile = dispMissile.Length();
	//	Vector3 velocityMissile = dispMissile * (speed / distance);

	//	Vector3 dP = target->GetAiManager()->serverPos - local->GetAiManager()->serverPos;
	//	Vector3 dV = velocityMissile - velocity;
	//	float time = (dP / dV).Length();
	//	//LOG("time %f", time);
	//	return PosAfterTime(target, castTime + time);
	//}

	// todo struct PredOutput result, hitchance,
	static Vector3 Preidct33(GameObject* local, GameObject* target, float range, float radius, float castTime, float speed, int collision)
	{
		if (!local || !target)
			return {};

		const Vector3 localPos = local->GetAiManager()->serverPos;
		AiManager* targetAi = target->GetAiManager();
		float distanceMissile = targetAi->serverPos.Distance(localPos);

		if (distanceMissile > range)
			return {};

		if (speed >= 9999.f)
		{
			return PosAfterTime(target, castTime, 0);
		}

		auto waypoints = targetAi->GetFuturePoints();
		if (waypoints.size() <= 1 || !targetAi->isMoving)
			return waypoints.front();

		float travelTime = (distanceMissile / speed) + castTime;
		Vector3 predictedPos = PosAfterTime(target, travelTime, radius );

		distanceMissile = predictedPos.Distance(localPos);
		float missileTime = (distanceMissile / speed) + castTime;

		// check how long does it take for missile to travel to player,
		// for that time check where he would be, and for the new position
		// calculate the misile time again, repeat until missile and player time match

		while (std::abs(travelTime - missileTime) > 0.01f) // 10ms tolerance
		{
			//LOG("%f", std::abs(travelTime - missileTime));

			travelTime = missileTime;
			predictedPos = PosAfterTime(target, travelTime, radius );
			//render.Circle3D(predictedPos, 30);
			distanceMissile = predictedPos.Distance(localPos);
			if (distanceMissile > range)
				return {};
			missileTime = (distanceMissile / speed) + castTime;
		}

		return predictedPos;
	}

	PredOutput GetPrediction(GameObject* local, GameObject* target, float range, float radius, float castTime, float speed, int collision)
	{
		if (!local || !target || !target->IsAlive() || !local->IsAlive()
			/*|| target->IsTargetable() || target->IsInvulnerable()*/)
			return {};

		if (local->teamId == target->teamId)
			return {};

		AiManager* targetAi = target->GetAiManager();

		if (range * 2 < local->GetAiManager()->serverPos.Distance(targetAi->serverPos))
			return {};

		/*if (extraDelay)
		{
			castTime += Ping / 2000.f + 0.06f;
		}*/

		if (targetAi->isDashing)
		{
			//PredictDashing();
		}
		else if (float stunDuration = target->GetBuffManager()->StunDuration();
			stunDuration > 0.f)
		{
			//GetImmobilePrediction(target, stunDuration, radius);
		}
	}

	//static PredOutput SkillInstant(GameObject* local, GameObject* target, float range, float radius, float castTime, int collision)
	//{
	//	if (!target || !local)
	//		return {};

	//	const Vector3 localPos = local->GetAiManager()->serverPos;
	//	const Vector3 targetPos = local->GetAiManager()->serverPos;

	//	PredOutput pred;

	//	if (localPos.Distance(targetPos) > (range + 300.f))
	//		return pred;

	//	//todo add dashing, buffs, check when auto attacking, collisions

	//	Vector3 predictionPos = PosAfterTime(target, castTime);

	//	if (localPos.Distance(predictionPos) > range)
	//		return pred;

	//	pred.pos = predictionPos;
	//	pred.hitChance = 5;

	//	return pred;
	//}
};