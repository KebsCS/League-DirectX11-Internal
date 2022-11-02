#pragma once

#include "Includes.h"
#include "Globals.h"
#include "HashFunctions.h"

enum class EmoteType : uint8_t;

namespace LeagueFuncs
{
	/*typedef void(__thiscall* tSendChat)(DWORD ChatClient, const char* Message, int Color);
	static void SendChat(const char* message, int color = 0)
	{
		static tSendChat fnSendChat = (tSendChat)(Globals::dwBaseAddress + oSendChat);
		fnSendChat(*(DWORD*)(Globals::dwBaseAddress + oChatClientPtr), message, color);
	}*/

	// could be unsafe, haven't checked
	//typedef void(__thiscall* tPrintChat)(DWORD ChatClient, const char* Message, int Color);
	//static void PrintChat(const char* message, int color = 0)
	//{
	//	static tPrintChat fnPrintChat = (tPrintChat)(Globals::dwBaseAddress + oPrintChat);
	//	fnPrintChat(*(DWORD*)(Globals::dwBaseAddress + oChatClientPtr), message, color);
	//}

	//typedef bool(__cdecl* tTestFunc)(Vector3*);
	//static bool TestFunc(Vector3 pos)
	//{
	//	static tTestFunc fnTestFunc = (tTestFunc)(Globals::dwBaseAddress + 0x9A96D0);
	//	return fnTestFunc(&pos);
	//}
	// 
	// TODO VIEWMATRIX IS WRONG
	static Matrix4x4 GetViewMatrix()
	{
		return *reinterpret_cast<Matrix4x4*>(Globals::dwBaseAddress + oViewMatrix);
	}

	static Matrix4x4 GetProjectionMatrix()
	{
		return *reinterpret_cast<Matrix4x4*>(Globals::dwBaseAddress + oViewMatrix + 0x40);
	}

	[[nodiscard]] static ImVec2 WorldToScreen(const Vector3& pos)
	{

		Vector4 clipCoords;
		clipCoords.x = pos.x * Globals::multipliedMatrix[0] + pos.y * Globals::multipliedMatrix[4] + pos.z * Globals::multipliedMatrix[8] + Globals::multipliedMatrix[12];
		clipCoords.y = pos.x * Globals::multipliedMatrix[1] + pos.y * Globals::multipliedMatrix[5] + pos.z * Globals::multipliedMatrix[9] + Globals::multipliedMatrix[13];
		clipCoords.z = pos.x * Globals::multipliedMatrix[2] + pos.y * Globals::multipliedMatrix[6] + pos.z * Globals::multipliedMatrix[10] + Globals::multipliedMatrix[14];
		clipCoords.w = pos.x * Globals::multipliedMatrix[3] + pos.y * Globals::multipliedMatrix[7] + pos.z * Globals::multipliedMatrix[11] + Globals::multipliedMatrix[15];

		//clipCoords.X = pos.X * matrix[0] + pos.Y * matrix[1] + pos.Z * matrix[2] + matrix[3];
		//clipCoords.Y = pos.X * matrix[4] + pos.Y * matrix[5] + pos.Z * matrix[6] + matrix[7];
		//clipCoords.Z = pos.X * matrix[8] + pos.Y * matrix[9] + pos.Z * matrix[10] + matrix[11];
		//clipCoords.W = pos.X * matrix[12] + pos.Y * matrix[13] + pos.Z * matrix[14] + matrix[15];

		Vector2 returnVec = { 0.f, 0.f };
		if (clipCoords.w < 0.1f) return returnVec;

		Vector3 temp;
		temp.x = clipCoords.x / clipCoords.w;
		temp.y = clipCoords.y / clipCoords.w;
		temp.z = clipCoords.z / clipCoords.w;

		// todo move this
		static DWORD dwRenderer = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oRenderer);
		static ImVec2 windowSize = ImVec2(*reinterpret_cast<int*>(dwRenderer + oRendererWidth), *reinterpret_cast<int*>(dwRenderer + oRendererHeight));

		returnVec.x = (windowSize.x / 2.0f * temp.x) + (temp.x + windowSize.x / 2.0f);
		returnVec.y = -(windowSize.y / 2.0f * temp.y) + (temp.y + windowSize.y / 2.0f);

		return returnVec;
	}

	//typedef bool(__cdecl* tWorldToScreen)(Vector3* in, Vector2* out);
	//static bool WorldToScreenGame(Vector3& in, Vector2& out)
	//{
	//	static tWorldToScreen fWorldToScreen = (tWorldToScreen)(Globals::dwBaseAddress + 0xA1AAF0);
	//	return fWorldToScreen(&in, &out);
	//}

	[[nodiscard]] static Vector3 GetMouseWorldPos()
	{
		//auto aux1 = *reinterpret_cast<DWORD*>(RVA(oHudInstance));
		//aux1 += 0x14;
		//auto aux2 = *reinterpret_cast<DWORD*>(aux1);
		//aux2 += 0x1C;
		//float X = *reinterpret_cast<float*>(aux2 + 0x0);
		//float Y = *reinterpret_cast<float*>(aux2 + 0x4);
		//float Z = *reinterpret_cast<float*>(aux2 + 0x8);
		//return Vector3{ X, Y, Z };

		static DWORD dwHudInstance = *reinterpret_cast<DWORD*>(RVA(oHudInstance));

		return *reinterpret_cast<Vector3*>(dwHudInstance + oMousePos);
	}

	static void DoEmote(EmoteType emote)
	{
		x86RetSpoof::invokeStdcall<bool>(RVA(oDoEmote), RVA(oSpoofGadget), emote);
		/*	reinterpret_cast<bool *(__stdcall*)
				(EmoteType)>(RVA(oDoEmote))
				(emote);*/
	}

	//[[nodiscard]] static int GetPing()
	//{
	//	static DWORD dwPingInstance = *reinterpret_cast<DWORD*>(RVA(oForGetPing));
	//	static DWORD dwPingPointer = *reinterpret_cast<DWORD*>(dwPingInstance + 0x3C);

	//	return *reinterpret_cast<int*>(dwPingPointer + 0x28);
	//}

	//todo implement this to spell classes
	static bool IsBasicAttackSlot(const int& slotId)
	{
		return (unsigned int)(slotId - 64) <= 17;
	}

	//static int GetPing()
	//{
	//	//3CD5F30
	//	//double v2 = x86RetSpoof::invokeThiscall<double>(*(DWORD*)(RVA(0x184E578) + 40), RVA(0x968CB0), RVA(oSpoofGadget));
	//	//LOG("%lf", v2);
	//	//v2 = 0.000835
	//	return x86RetSpoof::invokeStdcall<signed int>(RVA(oGetPing), RVA(oSpoofGadget), RVA(0x30F5F30), 0.f);
	//}

	static unsigned GetCollisionFlags(float* a1)
	{
		// inside IsNotWall, or IsGrass etc
		DWORD dword_42AFB10 = *reinterpret_cast<DWORD*>(RVA(oIsNotWallOffset));

		float v2; // xmm1_4
		int v3; // ecx
		int v4; // edx
		signed int v5; // edi
		float v6; // xmm0_4
		int v7; // eax
		int v8; // edx
		int v9; // eax
		__int16 v10; // cx
		int v11; // ecx
		__int16 v13; // ax

		v2 = *(float*)(dword_42AFB10 + 1452);
		v3 = *(DWORD*)(dword_42AFB10 + 1440);
		v4 = v3 - 1;
		v5 = (signed int)(float)((float)(a1[2] - *(float*)(dword_42AFB10 + 108)) * v2);
		v6 = (float)(a1[0] - *(float*)(dword_42AFB10 + 100)) * v2;
		if ((signed int)v6 <= v3 - 1)
		{
			v4 = (signed int)v6;
			if ((signed int)v6 < 0)
				v4 = 0;
		}
		v7 = *(DWORD*)(dword_42AFB10 + 1444) - 1;
		if (v5 <= v7)
		{
			v7 = (signed int)(float)((float)(a1[2] - *(float*)(dword_42AFB10 + 108)) * v2);
			if (v5 < 0)
				v7 = 0;
		}
		v8 = *(DWORD*)(dword_42AFB10 + 128) + 8 * (v4 + v7 * v3);
		if (!v8)
			return 0;
		v9 = *reinterpret_cast<DWORD*>(v8);
		if (*reinterpret_cast<DWORD*>(v8))
			v10 = *reinterpret_cast<WORD*>(v9 + 6);
		else
			v10 = *reinterpret_cast<WORD*>(v8 + 4);
		v11 = v10 & 0xC00;
		if (v11)
			return v11;
		if (v9)
			v13 = *reinterpret_cast<WORD*>(v9 + 6);
		else
			v13 = *reinterpret_cast<WORD*>(v8 + 4);
		return v13;
	}

	static bool IsBrush(Vector3 pos)
	{
		return GetCollisionFlags(pos) & CollisionFlags::Grass;
	}

	static bool IsNotWall(Vector3 pos)
	{
		return !(GetCollisionFlags(pos) & CollisionFlags::Wall);
	}

	static int FindVFunc(const DWORD& object, const DWORD& address)
	{
#ifdef DEVELOPER
		DWORD* base = *reinterpret_cast<DWORD**>(object);
		size_t i = 0;
		while (base[i])
		{
			if ((base[i] - Globals::dwBaseAddress) == address)
			{
				LOG("Found function at %d", i);
				return i;
			}
			i++;
		}
		LOG("Function not found %d", i);
		return -1;
#endif
	}
}

enum class EmoteType : uint8_t
{
	Dance = 0,
	Taunt = 1,
	Laugh = 2,
	Joke = 3,
	Toggle = 4
};