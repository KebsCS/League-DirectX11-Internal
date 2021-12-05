#pragma once

#include "Includes.h"
#include "Offsets.h"
#include "GameObject.h"

class ObjectManager
{
private:

public:

	// todo, init heroes once on load
	static std::list<GameObject*>HeroList()
	{
		std::list<GameObject*>objList;

		DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oHeroList);
		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		int nArrayLen = *reinterpret_cast<int*>(pList + 0x08);

		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back((GameObject*)(*reinterpret_cast<DWORD*>(pArray + i)));
		}
		return objList;
	}

	static std::list<GameObject*>MinionList()
	{
		std::list<GameObject*>objList;

		DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oMinionList);
		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		int nArrayLen = *reinterpret_cast<int*>(pList + 0x08);

		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back((GameObject*)(*reinterpret_cast<DWORD*>(pArray + i)));
		}
		return objList;
	}
};