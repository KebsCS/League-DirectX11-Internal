#pragma once

#include <string>
#include <Windows.h>

#define M_PI	3.14159265358979323846264338327950288419716939937510
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI) )

// Relative Virtual Address
#define RVA(address) (Globals::dwBaseAddress + address)

class LolString
{
	char content[0x10]; // 0x0
	int len = 0; // 0x10
	int max = 0; // 0x14

public:

	operator const char* (void)
	{
		return c_str();
	}
	operator std::string(void)
	{
		return std::string(c_str());
	}

private:
	char* c_str(void)
	{
		if (DWORD(this) <= 0x1000)
			return (char*)"";

		return max >= 0x10
			? *reinterpret_cast<char**>(content)
			: content;
	}
};