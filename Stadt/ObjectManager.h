#pragma once

#include "Includes.h"
#include "Offsets.h"

class ObjectManager
{
private:

public:

	// todo, init heroes once on load
	static std::list<DWORD/*GameObject**/>HeroList()
	{
		std::list<DWORD/*GameObject**/>objList;

		DWORD pHeroList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oHeroList);
		DWORD pHeroArray = *reinterpret_cast<DWORD*>(pHeroList + 0x04);
		int nHeroArrayLen = *reinterpret_cast<int*>(pHeroList + 0x08);

		for (int i = 0; i < nHeroArrayLen * 4; i += 4)
		{
			objList.emplace_back(*reinterpret_cast<DWORD*>(pHeroArray + i));
		}
		return objList;
	}
};