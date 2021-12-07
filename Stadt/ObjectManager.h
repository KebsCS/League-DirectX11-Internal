#pragma once

#include "Includes.h"
#include "Offsets.h"
#include "GameObject.h"

class ObjectManager
{
private:
	static std::vector<GameObject*>tempHeroList;

	static DWORD GetFirst(DWORD objectManager)
	{
		DWORD v1; // eax
		DWORD v2; // edx

		v1 = *reinterpret_cast<DWORD*>(objectManager + 0x14);
		v2 = *reinterpret_cast<DWORD*>(objectManager + 0x18);
		if (v1 == v2)
			return 0;
		while (*(BYTE*)v1 & 1 || !*(DWORD*)v1)
		{
			v1 += 4;
			if (v1 == v2)
				return 0;
		}
		return *(DWORD*)v1;
	}

	static DWORD GetNext(DWORD objectManager, DWORD obj)
	{
		DWORD v2; // eax
		unsigned int v3; // edx
		unsigned int v4; // esi
		DWORD v5; // eax

		v2 = *reinterpret_cast<DWORD*>(objectManager + 0x14);
		v3 = *(unsigned __int16*)(obj + 32) + 1;
		v4 = (*reinterpret_cast<DWORD*>(objectManager + 0x18) - v2) >> 2;
		if (v3 >= v4)
			return 0;
		v5 = v2 + 4 * v3;
		while (*(BYTE*)v5 & 1 || !*(DWORD*)v5)
		{
			++v3;
			v5 += 4;
			if (v3 >= v4)
				return 0;
		}
		return *(DWORD*)v5;
	}

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

	[[nodiscard]] static std::vector<GameObject*>ObjectList()
	{
		static DWORD dwObjectManager = *reinterpret_cast<DWORD*>(RVA(oObjectManager));
		size_t nObjects = *reinterpret_cast<size_t*>(dwObjectManager + 0x8);

		std::vector<GameObject*>objList;
		objList.reserve(nObjects);
		DWORD obj = GetFirst(dwObjectManager);
		while (obj)
		{
			objList.emplace_back((GameObject*)obj);

			obj = GetNext(dwObjectManager, obj);
		}

		return objList;
	}
};

// linker error fix
std::vector<GameObject*> ObjectManager::tempHeroList;