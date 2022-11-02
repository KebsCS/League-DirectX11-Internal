#pragma once

#include "Offsets.h"
#include "Definitions.h"

class ScriptBaseBuff
{
public:
	char pad_0000[8]; //0x0000
	char name[32]; //0x0008
	char pad_0028[96]; //0x0028
	int32_t hashName; //0x0088
	char pad_008C[20]; //0x008C
};

class BuffInstance
{
public:
	char pad_0000[4]; //0x0000
	BuffType type; //0x0004
	ScriptBaseBuff* scriptBaseBuff; //0x0008
	float startTime; //0x000C
	float endTime; //0x0010
	float duration; //0x0014
	char pad_0018[8]; //0x0018
	uint32_t stacksAlt; //0x0020
	uint32_t stacksAlt2; //0x0024
	char pad_0028[8]; //0x0028
	float stacksFloat; //0x0030
	char pad_0034[64]; //0x0034
	int32_t stacks2; //0x0074
	int32_t stacks3; //0x0078
	char pad_007C[44]; //0x007C

	int AltStackCount() //
	{
		return (stacksAlt2 - stacksAlt) >> 3;
	}
};

class BuffManager
{
public:
	DWORD pad;

	std::vector<BuffInstance*> GetBuffList()
	{
		std::vector<BuffInstance*> buffs;
		const DWORD buffBegin = *reinterpret_cast<DWORD*>((DWORD)this + 0x10);
		const DWORD buffEnd = *reinterpret_cast<DWORD*>((DWORD)this + 0x14);

		if (buffBegin != 0 && buffEnd != 0)
		{
			for (DWORD i = 0; i < (buffEnd - buffBegin); i += 0x08)
			{
				BuffInstance* buff = *reinterpret_cast<BuffInstance**>(buffBegin + i);

				if ((DWORD)buff < 0x1000)
					continue;

				if ((DWORD)buff->scriptBaseBuff < 0x1000)
					continue;

				if (buff->AltStackCount() <= 0)
					continue;

				if (*reinterpret_cast<float*>(RVA(oGameTime)) > buff->endTime)
					continue;

				if (*reinterpret_cast<float*>(RVA(oGameTime)) < buff->startTime)
					continue;

				buffs.emplace_back(buff);
			}
		}

		return buffs;
	}

	bool HasBuff(const std::string& name)
	{
		for (auto& buff : GetBuffList())
		{
			if (crt::strcmp(buff->scriptBaseBuff->name, name.c_str()) == 0)
				return true;
		}
		return false;
	}

	float StunDuration()
	{
		float max = *reinterpret_cast<float*>(RVA(oGameTime));
		for (auto& buff : this->GetBuffList())
		{
			if (buff->type == BuffType::Charm || buff->type == BuffType::Knockup || buff->type == BuffType::Stun
				|| buff->type == BuffType::Suppression || buff->type == BuffType::Snare)
				if (max < buff->endTime)
					max = buff->endTime;
		}
		return (max - *reinterpret_cast<float*>(RVA(oGameTime)));
	}
};
