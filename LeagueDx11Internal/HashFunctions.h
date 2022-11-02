#pragma once

#include <cstring> // strlen

namespace HashFunctions
{
	// dragon names, champion names
	[[nodiscard]] const static int HashStringSDBM(const char* a1)
	{
		int v1; // esi
		char v2; // ah
		int i; // ecx
		char v4; // dl

		v1 = 0;
		if (a1)
		{
			v2 = *a1;
			for (i = (int)(a1 + 1); v2; v1 = v4 + 65599 * v1)
			{
				++i;
				v4 = v2 + 32;
				if ((unsigned __int8)(v2 - 65) > 0x19u)
					v4 = v2;
				v2 = *(BYTE*)(i - 1);
			}
		}
		return v1;
	}

	// constexpr dragon names, champion names
	[[nodiscard]] constexpr static int ConstHashStringSDBM(const char* a1)
	{
		int v1; // esi
		char v2; // ah
		int i; // ecx
		char v4; // dl

		v1 = 0;
		if (a1)
		{
			v2 = *a1;
			for (i = (int)(a1 + 1); v2; v1 = v4 + 65599 * v1)
			{
				++i;
				v4 = v2 + 32;
				if ((unsigned __int8)(v2 - 65) > 0x19u)
					v4 = v2;
				v2 = *(BYTE*)(i - 1);
			}
		}
		return v1;
	}

	// buff names
	[[nodiscard]] const static int HashStringCaseInsensitiveFNV1a(const char* a1, int a3 = -2128831035)
	{
		int a2 = crt::strlen(a1);
		const char* v3; // esi
		int v4; // edi
		int v5; // ebp
		char v6; // bl
		char v7; // cl
		int v8; // eax

		v3 = a1;
		v4 = a2;
		if (!a2)
			return a3;
		v5 = a3;
		do
		{
			v6 = *v3++;
			v7 = v6 + 32;
			if ((unsigned __int8)(v6 - 65) > 0x19u)
				v7 = v6;
			v8 = v5 ^ v7;
			v5 = 16777619 * v8;
			--v4;
		} while (v4);
		return 16777619 * v8;
	}

	[[nodiscard]] const static int HashStringFNV1a(const char* a1, int a2 = -2128831035)
	{
		const char* v2; // edx
		unsigned int v3; // ecx
		int i; // esi
		int v5; // eax

		v2 = a1;
		v3 = crt::strlen(a1);
		for (i = a2; v3; --v3)
		{
			v5 = *(unsigned __int8*)v2++;
			i = 16777619 * (i ^ v5);
		}
		return i;
	}

	[[nodiscard]] const static int HashStringCaseInsensitiveFNV(const char* a1)
	{
		const char* v1; // esi
		int v2; // edi
		char i; // al
		char v4; // cl

		v1 = a1;
		v2 = -2128831035;
		for (i = *a1; *v1; v2 = v4 ^ 16777619 * v2)
		{
			v4 = i + 32;
			++v1;
			if ((unsigned __int8)(i - 65) > 0x19u)
				v4 = i;
			i = *v1;
		}
		return v2;
	}

	// GetHashedGameObjName
	// a2 is "*" in Riot::File::ConfigFile::AutoHash::AutoHash
	[[nodiscard]] const static int HashStringSDBM2(const char* a1, const char* a2 = "_gobj")
	{
		int v2; // edi
		char v3; // bl
		const char* i; // esi
		char v5; // cl
		bool v6; // cf
		bool v7; // zf
		char v8; // al
		char v9; // bl
		const char* j; // esi
		char v11; // cl
		char v12; // al

		v2 = 0;
		if (!a1)
			return 0;
		v3 = *a1;
		for (i = a1 + 1; v3; v2 = v5 + 65599 * v2)
		{
			v5 = v3 + 32;
			v6 = (unsigned __int8)(v3 - 65) < 0x19u;
			v7 = v3 == 90;
			v8 = v3;
			v3 = *i++;
			if (!v6 && !v7)
				v5 = v8;
		}
		v9 = *a2;
		for (j = a2 + 1; v9; v2 = v11 + 65599 * v2)
		{
			v11 = v9 + 32;
			v6 = (unsigned __int8)(v9 - 65) < 0x19u;
			v7 = v9 == 90;
			v12 = v9;
			v9 = *j++;
			if (!v6 && !v7)
				v11 = v12;
		}
		return v2;
	}

	// used for config files
	[[nodiscard]] const static int HashStringSDBM3(const char* a1, const char* a3)
	{
		char a2 = 42;
		int v3; // edi
		char v4; // bl
		const char* i; // esi
		char v6; // cl
		bool v7; // cf
		bool v8; // zf
		char v9; // al
		char v10; // bl
		const char* v11; // esi
		int j; // edi
		char v13; // cl
		char v14; // al

		v3 = 0;
		if (!a1)
			return 0;
		v4 = *a1;
		for (i = a1 + 1; v4; v3 = v6 + 65599 * v3)
		{
			v6 = v4 + 32;
			v7 = (unsigned __int8)(v4 - 65) < 0x19u;
			v8 = v4 == 90;
			v9 = v4;
			v4 = *i++;
			if (!v7 && !v8)
				v6 = v9;
		}
		v10 = *a3;
		v11 = a3 + 1;
		for (j = a2 + 65599 * v3; v10; j = v13 + 65599 * j)
		{
			v13 = v10 + 32;
			v7 = (unsigned __int8)(v10 - 65) < 0x19u;
			v8 = v10 == 90;
			v14 = v10;
			v10 = *v11++;
			if (!v7 && !v8)
				v13 = v14;
		}
		return j;
	}

	// related to spells and animations
	[[nodiscard]] const static int HashStringELF(const char* a1)
	{
		const char* v1; // esi
		int result; // eax
		char i; // ch
		char v4; // dl

		v1 = a1;
		result = 0;
		for (i = *a1; i; ++v1)
		{
			v4 = i + 32;
			if ((unsigned __int8)(i - 65) > 0x19u)
				v4 = i;
			result = v4 + 16 * result;
			if (result & 0xF0000000)
				result ^= result & 0xF0000000 ^ ((result & 0xF0000000) >> 24);
			i = v1[1];
		}
		return result;
	}
}