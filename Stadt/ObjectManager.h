#pragma once

#include "Includes.h"
#include "Offsets.h"
#include "GameObject.h"

class ObjectManager
{
private:
	static std::vector<GameObject*>tempHeroList;

public:

	[[nodiscard]] static std::vector<GameObject*>HeroList()
	{
		if (tempHeroList.empty())
		{
			DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oHeroList);
			DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
			int nArrayLen = *reinterpret_cast<int*>(pList + 0x08);

			tempHeroList.reserve(nArrayLen);
			for (auto i = 0; i < nArrayLen * 4; i += 4)
			{
				tempHeroList.emplace_back((GameObject*)(*reinterpret_cast<DWORD*>(pArray + i)));
			}
		}

		return tempHeroList;
	}

	[[nodiscard]] static std::vector<GameObject*>MinionList()
	{
		std::vector<GameObject*>objList;

		DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oMinionList);
		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		int nArrayLen = *reinterpret_cast<int*>(pList + 0x08);

		objList.reserve(nArrayLen);
		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back((GameObject*)(*reinterpret_cast<DWORD*>(pArray + i)));
		}
		return objList;
	}
};

// linker error fix
std::vector<GameObject*> ObjectManager::tempHeroList;