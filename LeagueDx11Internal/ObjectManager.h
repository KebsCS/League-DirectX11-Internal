#pragma once

#include "Includes.h"
#include "Offsets.h"
#include "GameObject.h"

class ObjectManager
{
private:
	inline static std::vector<GameObject*>tempHeroList;

	static DWORD GetFirst(const DWORD& objectManager)
	{
		//DWORD v1; // eax
		//DWORD v2; // edx

		static DWORD v1 = *reinterpret_cast<DWORD*>(objectManager + 0x14);
		static DWORD v2 = *reinterpret_cast<DWORD*>(objectManager + 0x18);
		if (v1 == v2)
			return 0;
		while (*reinterpret_cast<BYTE*>(v1) & 1 || !*reinterpret_cast<DWORD*>(v1))
		{
			v1 += 4;
			if (v1 == v2)
				return 0;
		}
		return *reinterpret_cast<DWORD*>(v1);
	}

	static DWORD GetNext(const DWORD& objectManager, const DWORD& obj)
	{
		//DWORD v2; // eax
		unsigned int v3; // edx
		//unsigned int v4; // esi
		DWORD v5; // eax

		static DWORD v2 = *reinterpret_cast<DWORD*>(objectManager + 0x14);
		v3 = *reinterpret_cast<unsigned __int16*>(obj + 32) + 1;
		static unsigned int v4 = (*reinterpret_cast<DWORD*>(objectManager + 0x18) - v2) >> 2;
		if (v3 >= v4)
			return 0;
		v5 = v2 + 4 * v3;
		while (*reinterpret_cast<BYTE*>(v5) & 1 || !*reinterpret_cast<DWORD*>(v5))
		{
			++v3;
			v5 += 4;
			if (v3 >= v4)
				return 0;
		}
		return *reinterpret_cast<DWORD*>(v5);
	}

public:

	[[nodiscard]] static std::vector<GameObject*>HeroList()
	{
		if (tempHeroList.empty())
		{
			static DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oHeroList);
			DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
			size_t nArrayLen = *reinterpret_cast<size_t*>(pList + 0x08);

			tempHeroList.reserve(nArrayLen);
			for (auto i = 0; i < nArrayLen * 4; i += 4)
			{
				tempHeroList.emplace_back(*reinterpret_cast<GameObject**>(pArray + i));
			}
		}

		return tempHeroList;
	}

	[[nodiscard]] static std::vector<GameObject*>MinionList()
	{
		std::vector<GameObject*>objList;

		static DWORD pList = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oMinionList);
		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		size_t nArrayLen = *reinterpret_cast<size_t*>(pList + 0x08);

		objList.reserve(nArrayLen);
		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back(*reinterpret_cast<GameObject**>(pArray + i));
		}
		return objList;
	}

	[[nodiscard]] static std::map<DWORD, GameObject*>ObjectMap()
	{
		return *reinterpret_cast<std::map<DWORD, GameObject*>*>(*reinterpret_cast<DWORD*>(RVA(oObjectManager)) + 0x28);
	}

	// todo, check if noexcept improves performance
	// Contains every object in the game
	[[nodiscard]] static std::vector<GameObject*>ObjectList() noexcept
	{
		static DWORD dwObjectManager = *reinterpret_cast<DWORD*>(RVA(oObjectManager));
		size_t nObjects = *reinterpret_cast<size_t*>(dwObjectManager + 0x8);

		std::vector<GameObject*>objList;
		objList.reserve(nObjects);
		DWORD obj = GetFirst(dwObjectManager);
		while (obj)
		{
			objList.emplace_back(reinterpret_cast<GameObject*>(obj));

			obj = GetNext(dwObjectManager, obj);
		}

		return objList;
	}

	// minions, players and structures?
	[[nodiscard]] static std::vector<GameObject*>SomeList() noexcept
	{
		/*static DWORD dwObjectManager = *reinterpret_cast<DWORD*>(RVA(oObjectManager));
		size_t nObjects = *reinterpret_cast<size_t*>(dwObjectManager + 0x8);*/

		static DWORD pList = *reinterpret_cast<DWORD*>(RVA(oObjectManager) + 0x78);

		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		size_t nArrayLen = *reinterpret_cast<size_t*>(pList + 0x08);

		std::vector<GameObject*>objList;
		objList.reserve(nArrayLen);
		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back(*reinterpret_cast<GameObject**>(pArray + i));
		}
		return objList;
	}

	// minions only?
	[[nodiscard]] static std::vector<GameObject*>SomeList2() noexcept
	{
		/*static DWORD dwObjectManager = *reinterpret_cast<DWORD*>(RVA(oObjectManager));
		size_t nObjects = *reinterpret_cast<size_t*>(dwObjectManager + 0x8);*/

		static DWORD pList = *reinterpret_cast<DWORD*>(RVA(oObjectManager) + 0x98);

		DWORD pArray = *reinterpret_cast<DWORD*>(pList + 0x04);
		size_t nArrayLen = *reinterpret_cast<size_t*>(pList + 0x08);

		std::vector<GameObject*>objList;
		objList.reserve(nArrayLen);
		for (auto i = 0; i < nArrayLen * 4; i += 4)
		{
			objList.emplace_back(*reinterpret_cast<GameObject**>(pArray + i));
		}
		return objList;
	}

	// some map but it seems empty?
	[[nodiscard]] static std::map<DWORD, GameObject*>SomeList3()
	{
		return *reinterpret_cast<std::map<DWORD, GameObject*>*>(*reinterpret_cast<DWORD*>(RVA(oObjectManager) + 0xA0));
	}
};