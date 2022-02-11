#pragma once

#include <Windows.h>

#include "Offsets.h"
#include "AiManager.h"
#include "BuffManager.h"
#include "Ntdefs.h"

enum class StatType : unsigned;
enum class StatInfo : uint8_t;

class GameObject
{
public:
	char pad_0000[4]; //0x0000 // dont remove this

	char pad_0004[28]; //0x0004
	uint32_t index; //0x0020
	char pad_0024[40]; //0x0024
	uint16_t teamId; //0x004C
	char pad_004E[30]; //0x004E
	LolString name; //0x006C
	char pad_0084[72]; //0x0084
	uint32_t networkId; //0x00CC
	char pad_00D0[292]; //0x00D0
	Vector3 position; //0x01F4
	char pad_0200[180]; //0x0200
	float mana; //0x02B4
	char pad_02B8[12]; //0x02B8
	float maxMana; //0x02C4
	char pad_02C8[2796]; //0x02C8
	float health; //0x0DB4
	char pad_0DB8[12]; //0x0DB8
	float maxhealth; //0x0DC4
	char pad_0DC8[164]; //0x0DC8
	class N0000300B* Funcs; //0x0E6C
	char pad_0E70[7540]; //0x0E70
	char* skinName; //0x2BE4
	char pad_2BFC[1180]; //0x2BFC

	std::string AddressHex()
	{
		return std::format(XorStr("{:#x}"), (DWORD)this);
	}

	// todo, rewrite to not use a call
	AiManager* GetAiManager() // GetPathController
	{
		return VFuncCall<AiManager*>(this, oVFunc::AiManager);
		/*int uVar2;
		int puVar3;
		int uStack4;
		int num1 = oAiManager - 4;
		uStack4 = *reinterpret_cast<int*>(this + (num1 + 8) + (*reinterpret_cast<byte*>(this + (num1 + 4))) * 4);
		puVar3 = (DWORD)this + num1;
		uVar2 = *reinterpret_cast<int*>(puVar3);
		uStack4 ^= ~uVar2;
		dwAiManager = *reinterpret_cast<DWORD*>(uStack4 + 8);

		return (AiManager*)(dwAiManager);*/
	}

	BuffManager* GetBuffManager()
	{
		//return VFuncCall<BuffManager*>(this, oVFunc::GetSpellBuffs);
		return reinterpret_cast<BuffManager*>((DWORD)this + oBuffManager);
	}

	bool IsLaneMinion()
	{
		if (!IsFunc(ObjectType::Minion))
			return false;
		return VFuncCall<bool>(this, oVFunc::IsLaneMinion);
	}

	bool IsClone()
	{
		if (!IsFunc(ObjectType::Minion))
			return false;
		return VFuncCall<bool>(this, oVFunc::IsClone);
	}

	bool IsAlive()
	{
		if (IsFunc(ObjectType::Minion) || IsFunc(ObjectType::Hero) ||
			IsFunc(ObjectType::Turret) || IsFunc(ObjectType::AttackableUnit) ||
			IsFunc(ObjectType::Building))
		{
			if (IsFunc(ObjectType::Hero))
			{
				return this->GetRespawnTimeRemaining() == 0.f;
			}
			return VFuncCall<bool>(this, oVFunc::ShouldDrawHealthBar);
		}
		return false;
	}

	bool IsTestFunc()
	{
		if (!IsFunc(ObjectType::Minion) && !IsFunc(ObjectType::Hero))
			return false;
		return VFuncCall<bool>(this, 58);
	}

	float GetBoundingRadius()
	{
		return VFuncCall<float>(this, oVFunc::BoundingRadius);
	}

	int IsFunc(const ObjectType& type)
	{
		if (!this)
			return 0;

		int uVar2;
		int puVar3;
		int uStack4;

		uStack4 = *reinterpret_cast<int*>((DWORD)this + 0x5c + (*reinterpret_cast<byte*>((DWORD)this + 0x58)) * 4);
		puVar3 = (DWORD)this + 0x54;
		uVar2 = *reinterpret_cast<int*>(puVar3);
		uStack4 ^= ~uVar2;

		return  (((int)type & uStack4) != 0);
	}

	bool IsJungleMonster()
	{
		//if (this->teamId != 100 && this->teamId != 200) return true;
		return VFuncCall<bool>(this, oVFunc::IsJungleMonster);
	}

	double GetRespawnTimeRemaining()
	{
		return VFuncCall<double>(this, oVFunc::GetRespawnTimeRemaining);
	}

	bool IsWard()
	{
		if (this->teamId != 100 && this->teamId != 200)
			return false;
		return VFuncCall<bool>(this, oVFunc::IsWard);
	}

	bool IsBaron()
	{
		if (!IsFunc(ObjectType::Minion))
			return false;
		return !strcmp(this->skinName, XorStr("Worm")) || !strcmp(this->skinName, XorStr("SRU_Baron"));
	}

	bool IsDragon()
	{
		if (!IsFunc(ObjectType::Minion))
			return false;
		return !strcmp(this->skinName, XorStr("Dragon")) || !strncmp(this->skinName, XorStr("SRU_Dragon"), 10);
	}

	double GetStatTotal(StatType type, StatInfo outputType = (StatInfo)0)
	{
		DWORD statOwner = (DWORD)this + 584;
		return x86RetSpoof::invokeThiscall<double>(statOwner, RVA(oGetStatTotal), RVA(oSpoofGadget), type, outputType);
	}
};

enum class StatType : unsigned
{
	AbilityPower = 0,
	Armor = 1,
	AttackDamage = 2,
	AttackSpeed = 3,
	MagicResist = 5,
	MoveSpeed = 6,
	CritChance = 7,
	CritDamageMultiplier = 8,
	//PercentCooldownMod,
	AbilityHaste = 10,
	Health = 11,
	HealthPercent = 12,
	HealthMissingPercent = 13,
	//SpellVamp,
	LifeSteal = 15,
	//MagicPercentPenetration,
	MagicFlatPenetration = 19,
	MagicBonusPercentPenetration = 20,
	//MagicLethality,
	//ArmorFlatPenetration,
	//ArmorPercentPenetration,
	ArmorBonusPercentPenetration = 24,
	//Unknown1,
	Lethality = 26,
	Tenacity = 27,
	AttackRange = 28,
	HealthRegen = 29,
	ManaRegen = 30,
	//Unknown2,
};

enum class StatInfo : uint8_t
{
	TOTAL = 0,
	BASE = 1,
	BONUS = 2,
};