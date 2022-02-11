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

	//typedef bool(__cdecl* tIsNotWall)(Vector3*, unsigned __int16);
	static bool IsNotWall(Vector3 pos)
	{
		return x86RetSpoof::invokeCdecl<bool>(RVA(oIsNotWall), RVA(oSpoofGadget),
			&pos, (unsigned __int16)0);
	}

	//typedef bool(__cdecl* tIsBrush)(Vector3*);
	static bool IsBrush(Vector3 pos)
	{
		return x86RetSpoof::invokeCdecl<bool>(RVA(oIsBrush), RVA(oSpoofGadget),
			&pos);
	}

	//typedef bool(__cdecl* tTestFunc)(Vector3*);
	//static bool TestFunc(Vector3 pos)
	//{
	//	static tTestFunc fnTestFunc = (tTestFunc)(Globals::dwBaseAddress + 0x9A96D0);
	//	return fnTestFunc(&pos);
	//}

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
		static float matrix[16];

		Matrix4x4::MultiplyMatrices(matrix, GetViewMatrix().matrix, GetProjectionMatrix().matrix);

		ImVec2 returnVec = ImVec2(0, 0);

		Vector4 clipCoords;
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
		clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
		clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
		clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

		//clipCoords.X = pos.X * matrix[0] + pos.Y * matrix[1] + pos.Z * matrix[2] + matrix[3];
		//clipCoords.Y = pos.X * matrix[4] + pos.Y * matrix[5] + pos.Z * matrix[6] + matrix[7];
		//clipCoords.Z = pos.X * matrix[8] + pos.Y * matrix[9] + pos.Z * matrix[10] + matrix[11];
		//clipCoords.W = pos.X * matrix[12] + pos.Y * matrix[13] + pos.Z * matrix[14] + matrix[15];

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
	}

	//[[nodiscard]] static int GetPing()
	//{
	//	static DWORD dwPingInstance = *reinterpret_cast<DWORD*>(RVA(oForGetPing));
	//	static DWORD dwPingPointer = *reinterpret_cast<DWORD*>(dwPingInstance + 0x3C);

	//	return *reinterpret_cast<int*>(dwPingPointer + 0x28);
	//}

	//todo implement this to spell classes
	static bool IsBasicAttackSlot(int slotId)
	{
		return (unsigned int)(slotId - 64) <= 17;
	}

	static size_t FindVFunc(DWORD object, DWORD address)
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
		//LOG("Function not found");
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