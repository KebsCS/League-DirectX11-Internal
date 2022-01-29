#pragma once

#include "Console.h"
#include "Offsets.h"

class ScriptBaseBuff
{
public:
	char pad_0000[8]; //0x0000
	char name[32]; //0x08
};

class BuffInstance
{
public:
	char pad_0000[4]; //0x0000
	int32_t type; //0x0004
	ScriptBaseBuff* scriptBaseBuff; //0x0008
	float startTime; //0x000C
	float endTime; //0x0010
	float duration; //0x0014
	char pad_0018[8]; //0x0018
	uint32_t stacksAlt; //0x0020
	uint32_t stacksAlt2; //0x0024
	char pad_0028[268]; //0x0028

	int StackCount()
	{
		return (stacksAlt2 - stacksAlt) >> 3;
	}
};

class BuffManager
{
public:
	DWORD test;

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

				if (buff->StackCount() <= 0)
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
			if (strcmp(buff->scriptBaseBuff->name, name.c_str()) == 0)
				return true;
		}
		return false;
	}
};