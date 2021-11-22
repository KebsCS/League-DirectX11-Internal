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

		DWORD pHeroList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oHeroList);
		DWORD pHeroArray = *reinterpret_cast<DWORD*>(pHeroList + 0x04);
		int nHeroArrayLen = *reinterpret_cast<int*>(pHeroList + 0x08);

		for (int i = 0; i < nHeroArrayLen * 4; i += 4)
		{
			objList.emplace_back((GameObject*)(*reinterpret_cast<DWORD*>(pHeroArray + i)));
		}
		return objList;
	}
};