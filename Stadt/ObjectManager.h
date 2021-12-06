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

	[[nodiscard]] static std::map<DWORD, GameObject*>ObjectMap()
	{
		return *reinterpret_cast<std::map<DWORD, GameObject*>*>(*reinterpret_cast<DWORD*>(RVA(oObjectManager)) + 0x28);
	}

	// todo, is this a list, vector, and what offset
	/*[[nodiscard]] static std::list<GameObject*>ObjectList()
	{
		return *reinterpret_cast<std::list<GameObject*>*>(*reinterpret_cast<DWORD*>(RVA(oObjectManager)) + 0x14);
	}*/
};

// linker error fix
std::vector<GameObject*> ObjectManager::tempHeroList;