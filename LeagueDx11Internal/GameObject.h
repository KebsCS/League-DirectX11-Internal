#pragma once

#include <Windows.h>

#include "Offsets.h"
#include "AiManager.h"
#include "BuffManager.h"
#include "Ntdefs.h"
#include "BaseClass.h"

enum class StatType : unsigned;
enum class StatInfo : uint8_t;

class GameObject
{
public:
	char pad_0000[4]; //0x0000 // dont remove this

	char pad_0004[28]; //0x0004
	uint32_t index; //0x0020
	char pad_0024[16]; //0x0024
	uint16_t teamId; //0x0034
	char pad_0036[30]; //0x0036
	LolString name; //0x0054
	char pad_006C[72]; //0x006C
	uint32_t networkId; //0x00B4
	char pad_00B8[292]; //0x00B8
	Vector3 position; //0x01DC
	char pad_01E8[180]; //0x01E8
	float mana; //0x029C
	char pad_02A0[12]; //0x02A0
	float maxMana; //0x02AC
	char pad_02B0[3012]; //0x02B0
	float health; //0x0E74
	char pad_0E78[12]; //0x0E78
	float maxhealth; //0x0E84
	char pad_0E88[168]; //0x0E88
	class N00004841* N000003F1; //0x0F30
	char pad_0F34[7280]; //0x0F34
	char* skinName; //0x2BA4
	char pad_2BA8[1392]; //0x2BA8


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
			LOGONCE("After IsFunc");
			if (IsFunc(ObjectType::Hero))
			{
				LOGONCE("Before GetRespawnTimeRemaining");

				return this->GetRespawnTimeRemaining() == 0.f;
			}
			LOGONCE("Before ShouldDrawHighlight");

			return VFuncCall<bool>(this, oVFunc::ShouldDrawHighlight);
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
		return !crt::strcmp(this->skinName, XorStr("Worm")) || !crt::strcmp(this->skinName, XorStr("SRU_Baron"));
	}

	bool IsDragon()
	{
		if (!IsFunc(ObjectType::Minion))
			return false;
		return !crt::strcmp(this->skinName, XorStr("Dragon")) || !strncmp(this->skinName, XorStr("SRU_Dragon"), 10);
	}

	double GetStatTotal(StatType type, StatInfo outputType = (StatInfo)0)
	{
		DWORD statOwner = (DWORD)this + 560;
		return x86RetSpoof::invokeThiscall<double>(statOwner, RVA(oGetStatTotal), RVA(oSpoofGadget), type, outputType);
	}

	//83 EC 14 8B 44 24 1C 55
	int GetSpellState(SpellSlotID slot)
	{
		return 0;//todo
		//DWORD spellbook = *(DWORD*)((DWORD)this + 0x2370);//VFuncCall<DWORD>(this, oVFunc::GetSpellbook);
		//LOG("spellbook %d", spellbook);
		//byte zero = 0;

		//return (x86RetSpoof::invokeThiscall<int>(std::uintptr_t(spellbook), RVA(0x4DD880), RVA(oSpoofGadget),
		//	(int)slot, &zero));
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
