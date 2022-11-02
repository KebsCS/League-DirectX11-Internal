#pragma once

#include "Includes.h"
#include "SpellCastInfo.h"
#include "GameObject.h"
#include "Geometry.h"

#include "VMTHook.h"

uintptr_t pOnProcessSpellCast;

VMTHook VMTHOnProcessSpellCast[10];

// AIHeroClient::CACHandleSpellSituations(Audio::SoundEventType,Spell::SpellCastInfo const&,AttackableUnit *)
DWORD* __fastcall OnProcessSpellCast(void* thisptr, void* edx, int state, SpellCastInfo* spellCastInfo, int a6) noexcept
{
	static auto fn = reinterpret_cast<DWORD * (__thiscall*)(void*, int, SpellCastInfo*, int)>(pOnProcessSpellCast);

	if (spellCastInfo != nullptr)
	{
		// todo an event manager
		std::string name = spellCastInfo->spellInfo->name;
		//ImVec2 startpos = LeagueFuncs::WorldToScreen(spellCastInfo->startPos);
		//ImVec2 endpos = LeagueFuncs::WorldToScreen(spellCastInfo->endPos);
		//Vector3 startpos = spellCastInfo->StartPos;
		std::stringstream ss;
		ss << std::hex << name << "  " << (DWORD)spellCastInfo << " ";
		/*<< startpos.x << " , " << startpos.y << " endpos: " << endpos.x << " , " << endpos.y;*/

		//testpos.emplace_back(spellCastInfo->endPos);

		GameObject* local = *reinterpret_cast<GameObject**>(RVA(oLocalPlayer));
		ss << "  id: " << std::dec << spellCastInfo->slotId;

		/*if (Globals::bMenuOpen)
		{
			std::stringstream ssAddy;
			ssAddy << name << "  " << std::hex << (DWORD)(spellCastInfo);
			MessageBoxA(0, ssAddy.str().c_str(), "", 0);
		}*/

		//ss << "  " << local->name << "  " << &local->networkId;
		if (name == "ThreshQ")
		{
			Vector3 extended = spellCastInfo->startPos.Extend(spellCastInfo->endPos, spellCastInfo->spellInfo->spellData->castRangeDisplayOverride);
			Vector3 sp = spellCastInfo->startPos;
			//extended.y = 100;
			//sp.y = 100;
			Geometry::Polygon poly = Geometry::Rectangle(sp, extended, 70.f).ToPolygon();

			//LeagueFuncs::SendChat(std::string((DWORD)(local->GetAiManager()->navBegin)).c_str());

			//testpoly.emplace_back(poly);
		}
		//LeagueFuncs::SendChat(ss.str().c_str());
		LOG(ss.str().c_str());
	}

	return fn(thisptr, state, spellCastInfo, a6);
}

void HookOnProcessSpellCast()
{
	LOG("Hooking OnProcessSpellCast...");

	for (int i = 0; auto & hero : ObjectManager::HeroList())
	{
		DWORD dwOnProcessSpell = (DWORD)hero + 0xF30;// 0xE6C;
		pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 29, (uintptr_t)&OnProcessSpellCast);
		i++;
	}

	LOG("Success");
}

void UnHookOnProcessSpellCast()
{
	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();

	LOG("Unhooked");
}