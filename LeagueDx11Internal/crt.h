#pragma once

namespace crt
{
	float sqrtf(float _X);
	float expf(float _X);

	float sinf(float _X);
	float cosf(float _X);

	float tanf(float _X);
	float asinf(float _X);
	float atan2f(float _X, float _Y);
	float atanf(float a);
	float acosf(float _X);

	int toupper(int c);
	int tolower(int c);

	void* memcpy(void* dest, const void* src, unsigned __int64 count);

	void* memset(void* src, int val, unsigned __int64 count);
	void* memccpy(void* to, const void* from, int c, unsigned __int64 count);
	void* memchr(const void* s, int c, unsigned __int64 n);
	int  memcmp(const void* s1, const void* s2, unsigned __int64 n);
	int  memicmp(const void* s1, const void* s2, unsigned __int64 n);
	void* memmove(void* dest, const void* src, unsigned __int64 count);

	char* _cslwr(char* x);
	char* _csupr(char* x);

	int strlen(const char* string);
	const char* strcpy(char* buffer, const char* string);
	const char* strcpy(char* buffer, const wchar_t* string);

	int strcmp(const char* cs, const char* ct);
	int stricmp(const char* cs, const char* ct);

	/*int stricmp(UCHAR cs, const char* ct);*/
	wchar_t* _cslwr(wchar_t* x);
	wchar_t* _csupr(wchar_t* x);

	int strlen(const wchar_t* string);
	const wchar_t* strcpy(wchar_t* buffer, const wchar_t* string);
	const wchar_t* strcpy(wchar_t* buffer, const char* string);

	int strcmp(const wchar_t* cs, const wchar_t* ct);
	int stricmp(const wchar_t* cs, const wchar_t* ct);

	int wtoupper(int c);
	int wtolower(int c);

	int towupper(int c);
	int towlower(int c);

	int wstrlen(const wchar_t* s);
	int wstrcmp(const wchar_t* cs, const wchar_t* ct);
	int wstricmp(const wchar_t* cs, const wchar_t* ct);
	wchar_t* wstrstr(const wchar_t* s, const wchar_t* find);
	wchar_t* wstrset(wchar_t* szToFill, int szFill);
	wchar_t* wstrnset(wchar_t* szToFill, int szFill, unsigned __int64 sizeMaxFill);
	wchar_t* wstrrev(wchar_t* s);
	int wstrnicmp(const wchar_t* s1, const wchar_t* s2, unsigned __int64 n);
	wchar_t* wstrlwr(wchar_t* x);
	wchar_t* wstrchr(const wchar_t* s, wchar_t c);
	wchar_t* wstrtok_s(wchar_t* str, const wchar_t* delim, wchar_t** ctx);
	bool iswdigit(wchar_t c);
	__int64 wtoi64(const wchar_t* nptr);
	int wtoi(const wchar_t* nptr);
	wchar_t* itow(int number, wchar_t* destination, int base);

	const char* strstr(char const* _Str, char const* _SubStr);

	char* strcatA(char* dest, const char* src);

	wchar_t* strcatW(wchar_t* dest, const wchar_t* src);
}