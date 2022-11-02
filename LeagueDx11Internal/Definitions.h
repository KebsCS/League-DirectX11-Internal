#pragma once

#include <string>
#include <Windows.h>

#define M_PI	3.14159265358979323846264338327950288419716939937510
#ifndef RAD2DEG
#define RAD2DEG(x)  ((float)(x) * (float)(180.f / M_PI_F))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

// Relative Virtual Address
#define RVA(address) (Globals::dwBaseAddress + address)

class LolString
{
	char content[0x10]; // 0x0
	int len = 0; // 0x10
	int max = 0; // 0x14

public:

	operator const char* (void)
	{
		return c_str();
	}
	operator std::string(void)
	{
		return std::string(c_str());
	}

	std::string str()
	{
		return std::string(c_str());
	}

private:
	char* c_str(void)
	{
		if (DWORD(this) <= 0x1000)
			return (char*)"";

		return max >= 0x10
			? *reinterpret_cast<char**>(content)
			: content;
	}
};

enum class ObjectType : unsigned
{
	//x << y = x*pow(2,y)
	//x >> y = x/pow(2,y)
	GameObject = (1 << 0),  //0x1
	NeutralCamp = (1 << 1),  //0x2
	DeadObject = (1 << 4),  //0x10
	InvalidObject = (1 << 5),  //0x20
	AIBaseCommon = (1 << 7),  //0x80
	AttackableUnit = (1 << 9),  //0x200
	AI = (1 << 10), //0x400
	Minion = (1 << 11), //0x800
	Hero = (1 << 12), //0x1000
	Turret = (1 << 13), //0x2000
	Unknown0 = (1 << 14), //0x4000
	Missile = (1 << 15), //0x8000
	Unknown1 = (1 << 16), //0x10000
	Building = (1 << 17), //0x20000
	Unknown2 = (1 << 18), //0x40000
	Unknown3 = (1 << 19), //0x80000
};

enum class ObjectTeam : unsigned
{
	Unknown = 0,
	Order = 100,
	Chaos = 200,
	Neutral = 300
};

enum class SpellSlotID : unsigned
{
	Q = 0,
	W = 1,
	E = 2,
	R = 3,
	D = 4,
	F = 5,
	Item1 = 6,
	Item2 = 7,
	Item3 = 8,
	Item4 = 9,
	Item5 = 10,
	Item6 = 11,
	Trinket = 12,
	Recall = 13,
	//SpecialAttack = 45,
	//JayceShockBlastWallMis = 46, // when Jayce Q gets improved by the gate, ViegoQDoubleAttack, DrMundoEAttack, EvelynnWMissile
	//PassiveProc = 47, // idk if correctly named, JaycePassiveMeleeAttack, ThreshPassiveSoulsVlad, ViegoRCast
	//  thresh has BasicAttack1S, 2S, 1M, 2M, but theye the same id
	// ViegoEMissile, ViegoPassiveAttack(possessing soul), EvelynnQLineMissile
	//ViegoEWallFollowMis = 48
	//ThreshBasicAttack1 = 49, // RyzeQ aswell
	KayleBasicAttack = 50, // every kayle basic attack is id 50
	// FleetFootwork_HealMis, RuinedKing_HealMis (grasp), BlessingOfTheLizardElderLines (obtaining red buff), CrestOfTheAncientGolemLines (blue buff)
	// PlantHawkshot (destroying vision plant), PerkSouldFeastVisualMissile (triumph?)
	//KeystoneProc = 52,
	//PassiveAttack = 53, // TalonPassiveAttack, idk if applies to everything
	//EvelynnRTrail =54,
	//JayceRangedAttack = 55,
	//PracticeTool_Teleport_Cast = 60,
	Passive = 63,
	BasicAttack1 = 64,
	BasicAttack2 = 65,
	BasicAttack3 = 66,
	BasicAttack4 = 67,
	CritAttack = 73,
	CritAttack2 = 74,
};

enum class BuffType : int
{
	Internal = 0,
	Aura = 1,
	CombatEnhancer = 2,
	CombatDehancer = 3,
	SpellShield = 4,
	Stun = 5,
	Invisibility = 6,
	Silence = 7,
	Taunt = 8,
	Berserk = 9,
	Polymorph = 10,
	Slow = 11,
	Snare = 12,
	Damage = 13,
	Heal = 14,
	Haste = 15,
	SpellImmunity = 16,
	PhysicalImmunity = 17,
	Invulnerability = 18,
	AttackSpeedSlow = 19,
	NearSight = 20,
	Fear = 22,
	Charm = 23,
	Poison = 24,
	Suppression = 25,
	Blind = 26,
	Counter = 27,
	Currency = 21,
	Shred = 28,
	Flee = 29,
	Knockup = 30,
	Knockback = 31,
	Disarm = 32,
	Grounded = 33,
	Drowsy = 34,
	Asleep = 35,
	Obscured = 36,
	ClickproofToEnemies = 37,
	UnKillable = 38
};

enum CollisionFlags : unsigned
{
	None = 0,
	Grass = 1,
	Wall = 2,
	Building = 64,
	Tower = 70,
	Prop = 128,
	GlobalVision = 256
};

enum GameMap : unsigned
{
	CrystalScar = 8,
	TwistedTreeline = 10,
	SummonersRift = 11,
	HowlingAbyss = 12
};

enum SpellState : unsigned
{
	Ready = 0,
	NotAvailable = 4,
	Surpressed = 8,
	NotLearned = 12,
	Processing = 24,
	Cooldown = 32,
	NoMana = 64,
};

constexpr float levelExp[18] = { 0.f, 280.f, 660.f, 1140.f, 1720.f, 2400.f, 3180.f, 4060.f,
5040.f, 6120.f, 7300.f, 8580.f, 9960.f, 11440.f, 13020.f, 14700.f, 16480.f, 18360.f };