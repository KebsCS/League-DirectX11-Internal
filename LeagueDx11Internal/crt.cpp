#include "crt.h"

#include <intrin.h>

#pragma warning(disable:4244)

float crt::sqrtf(float _X) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X))); }
float crt::expf(float _X) { return _mm_cvtss_f32(_mm_exp_ps(_mm_set_ss(_X))); }

float crt::sinf(float _X) { return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X))); }
float crt::cosf(float _X) { return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X))); }

float crt::tanf(float _X) { return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(_X))); }
float crt::asinf(float _X) { return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(_X))); }
float crt::atan2f(float _X, float _Y) { return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(_X), _mm_set_ss(_Y))); }
float crt::atanf(float a) {return atan2f(a, 1.f);}
float crt::acosf(float _X) { return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(_X))); }

int crt::toupper(int c)
{
	if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
	return c;
}
int crt::tolower(int c)
{
	if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
	return c;
}

 void* crt::memcpy(void* dest, const void* src, unsigned __int64 count)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;
	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}
	return dest;
}

 void* crt::memset(void* src, int val, unsigned __int64 count)
{
	__stosb((unsigned char*)((unsigned long long)(volatile char*)src), val, count);
	return src;
}
void* crt::memccpy(void* to, const void* from, int c, unsigned __int64 count)
{
	char t;
	unsigned __int64 i;
	char* dst = (char*)to;
	const char* src = (const char*)from;
	for (i = 0; i < count; i++)
	{
		dst[i] = t = src[i];
		if (t == 0) break;
		if (t == c) return &dst[i + 1];
	}
	return 0;
}
void* crt::memchr(const void* s, int c, unsigned __int64 n)
{
	if (n)
	{
		const char* p = (const char*)s;
		do
		{
			if (*p++ == c) return (void*)(p - 1);
		} while (--n != 0);
	}
	return 0;
}
 int crt::memcmp(const void* s1, const void* s2, unsigned __int64 n)
{
	if (n != 0)
	{
		const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
		do
		{
			if (*p1++ != *p2++) return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return 0;
}
int  crt::memicmp(const void* s1, const void* s2, unsigned __int64 n)
{
	if (n != 0)
	{
		const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
		do
		{
			if (crt::toupper(*p1) != crt::toupper(*p2)) return (*p1 - *p2);
			p1++;
			p2++;
		} while (--n != 0);
	}
	return 0;
}
 void* crt::memmove(void* dest, const void* src, unsigned __int64 count)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;
	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}
	return dest;
}

char* crt::_cslwr(char* x)
{
	char* y = x;
	while (*y)
	{
		*y = crt::tolower(*y);
		y++;
	}
	return x;
}
 char* crt::_csupr(char* x)
{
	char* y = x;
	while (*y)
	{
		*y = crt::tolower(*y);
		y++;
	}
	return x;
}

  int crt::strlen(const char* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}
 const char* crt::strcpy(char* buffer, const char* string)
{
	char* ret = buffer;
	while (*string) *buffer++ = *string++;
	*buffer = 0;
	return ret;
}
  const char* crt::strcpy(char* buffer, const wchar_t* string)
{
	char* ret = buffer;
	while (*string) *buffer++ = char(*string++);
	*buffer = 0;
	return ret;
}

 int crt::strcmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}
int crt::stricmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (crt::tolower(*cs) == crt::tolower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return crt::tolower(*cs) - crt::tolower(*ct);
	}
	return -1;
}

/*int stricmp(UCHAR cs, const char* ct)
{
	if (ct)
	{
		while (crt::tolower(cs) == crt::tolower(*ct))
		{
			if (cs == 0 && *ct == 0) return 0;
			if (cs == 0 || *ct == 0) break;

			ct++;
		}
		return crt::tolower(cs) - crt::tolower(*ct);
	}
	return -1;
}*/
wchar_t* crt::_cslwr(wchar_t* x)
{
	wchar_t* y = x;
	while (*y)
	{
		*y = crt::towlower(*y);
		y++;
	}
	return x;
}
 wchar_t* crt::_csupr(wchar_t* x)
{
	wchar_t* y = x;
	while (*y)
	{
		*y = crt::towupper(*y);
		y++;
	}
	return x;
}

 int crt::strlen(const wchar_t* string)
{
	int cnt = 0;
	if (string)
	{
		for (; *string != 0; ++string) ++cnt;
	}
	return cnt;
}
const wchar_t* crt::strcpy(wchar_t* buffer, const wchar_t* string)
{
	wchar_t* ret = buffer;
	while (*string) *buffer++ = *string++;
	*buffer = L'\0';
	return ret;
}
const wchar_t* crt::strcpy(wchar_t* buffer, const char* string)
{
	wchar_t* ret = buffer;
	while (*string) *buffer++ = wchar_t(*string++);
	*buffer = 0;
	return ret;
}

int crt::strcmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}
  int crt::stricmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (crt::towlower(*cs) == crt::towlower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return crt::towlower(*cs) - crt::towlower(*ct);
	}
	return -1;
}

 int crt::wtoupper(int c)
{
	if (c >= L'a' && c <= L'z') return c - L'a' + L'A';
	if (c >= L'à' && c <= L'ÿ') return c - L'à' + L'À';
	if (c == L'¸') return L'¨';
	return c;
}
 int crt::wtolower(int c)
{
	if (c >= L'A' && c <= L'Z') return c - L'A' + L'a';
	if (c >= L'À' && c <= L'ß') return c - L'À' + L'à';
	if (c == L'¨') return L'¸';
	return c;
}

 int crt::towupper(int c)
{
	if (c >= L'a' && c <= L'z') return c - L'a' + L'A';
	if (c >= L'à' && c <= L'ÿ') return c - L'à' + L'À';
	if (c == L'¸') return L'¨';
	return c;
}
int crt::towlower(int c)
{
	if (c >= L'A' && c <= L'Z') return c - L'A' + L'a';
	if (c >= L'À' && c <= L'ß') return c - L'À' + L'à';
	if (c == L'¨') return L'¸';
	return c;
}
 int crt::wstrlen(const wchar_t* s)
{
	int cnt = 0;
	if (!s) return 0;
	for (; *s != 0; ++s) ++cnt;
	return cnt;
}
  int crt::wstrcmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}
int crt::wstricmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (crt::wtolower(*cs) == crt::wtolower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return crt::wtolower(*cs) - crt::wtolower(*ct);
	}
	return -1;
}
wchar_t* crt::wstrstr(const wchar_t* s, const wchar_t* find)
{
	wchar_t c, sc;
	if ((c = *find++) != 0)
	{
		do
		{
			do
			{
				if ((sc = *s++) == 0)
					return 0;
			} while (sc != c);
		} while (crt::wstricmp(s, find) != 0);
		s--;
	}
	return (wchar_t*)s;
}
 wchar_t* crt::wstrset(wchar_t* szToFill, int szFill)
{
	wchar_t* t = szToFill;
	while (*szToFill != 0)
	{
		*szToFill = szFill;
		szToFill++;
	}
	return t;
}
wchar_t* crt::wstrnset(wchar_t* szToFill, int szFill, unsigned __int64 sizeMaxFill)
{
	wchar_t* t = szToFill;
	int i = 0;
	while (*szToFill != 0 && i < (int)sizeMaxFill)
	{
		*szToFill = szFill;
		szToFill++;
		i++;
	}
	return t;
}
  wchar_t* crt::wstrrev(wchar_t* s)
{
	wchar_t a, * b, * e;
	b = e = s;
	while (*e) e++;
	e--;
	while (b < e)
	{
		a = *b;
		*b = *e;
		*e = a;
		b++;
		e--;
	}
	return s;
}
 int crt::wstrnicmp(const wchar_t* s1, const wchar_t* s2, unsigned __int64 n)
{
	if (n == 0) return 0;
	do
	{
		if (crt::wtoupper(*s1) != crt::wtoupper(*s2++)) return crt::wtoupper(*(unsigned const char*)s1) - crt::wtoupper(*(unsigned const char*)--s2);
		if (*s1++ == 0) break;
	} while (--n != 0);
	return 0;
}
 wchar_t* crt::wstrlwr(wchar_t* x)
{
	wchar_t* y = x;
	while (*y)
	{
		*y = crt::wtolower(*y);
		y++;
	}
	return x;
}
 wchar_t* crt::wstrchr(const wchar_t* s, wchar_t c)
{
	wchar_t cc = c;
	const wchar_t* sp = (wchar_t*)0;
	while (*s)
	{
		if (*s == cc) sp = s;
		s++;
	}
	if (cc == 0) sp = s;
	return (wchar_t*)sp;
}
 wchar_t* crt::wstrtok_s(wchar_t* str, const wchar_t* delim, wchar_t** ctx)
{
	if (!str) str = *ctx;
	while (*str && crt::wstrchr(delim, *str)) str++;
	if (!*str)
	{
		*ctx = str;
		return 0;
	}
	*ctx = str + 1;
	while (**ctx && !crt::wstrchr(delim, **ctx)) (*ctx)++;
	if (**ctx) *(*ctx)++ = 0;
	return str;
}
  bool crt::iswdigit(wchar_t c) { return c >= L'0' && c <= L'9'; }
 __int64 crt::wtoi64(const wchar_t* nptr)
{
	wchar_t* s = (wchar_t*)nptr;
	__int64 acc = 0;
	int neg = 0;
	if (nptr == 0) return 0;
	// while (*s = L' ') s++;
	if (*s == L'-')
	{
		neg = 1;
		s++;
	}
	else if (*s == L'+') s++;
	while (crt::iswdigit(*s))
	{
		acc = 10 * acc + (*s - L'0');
		s++;
	}
	if (neg) acc *= -1;
	return acc;
}
int crt::wtoi(const wchar_t* nptr)
{
	wchar_t* s = (wchar_t*)nptr;
	int acc = 0;
	int neg = 0;
	if (nptr == 0) return 0;
	// while (*s = L' ') s++;
	if (*s == L'-')
	{
		neg = 1;
		s++;
	}
	else if (*s == L'+') s++;
	while (crt::iswdigit(*s))
	{
		acc = 10 * acc + (*s - L'0');
		s++;
	}
	if (neg) acc *= -1;
	return acc;
}
wchar_t* crt::itow(int number, wchar_t* destination, int base)
{
	int count = 0;
	do
	{
		int digit = number % base;
		destination[count++] = (digit > 9) ? digit - 10 + L'A' : digit + L'0';
	} while ((number /= base) != 0);
	destination[count] = 0;
	int i;
	for (i = 0; i < count / 2; ++i)
	{
		wchar_t symbol = destination[i];
		destination[i] = destination[count - i - 1];
		destination[count - i - 1] = symbol;
	}
	return destination;
}

const char* crt::strstr(char const* _Str, char const* _SubStr)
{
	const char* bp = _SubStr;
	const char* back_pos;
	while (*_Str != 0 && _Str != 0 && _SubStr != 0)
	{
		back_pos = _Str;
		while (crt::tolower(*back_pos++) == crt::tolower(*_SubStr++))
		{
			if (*_SubStr == 0)
			{
				return (char*)(back_pos - crt::strlen(bp));
			}
		}
		++_Str;
		_SubStr = bp;
	}
	return 0;
}

  char* crt::strcatA(char* dest, const char* src)
{
	if ((dest == 0) || (src == 0))
		return dest;

	while (*dest != 0)
		dest++;

	while (*src != 0) {
		*dest = *src;
		dest++;
		src++;
	}

	*dest = 0;
	return dest;
}

wchar_t* crt::strcatW(wchar_t* dest, const wchar_t* src)
{
	if ((dest == 0) || (src == 0))
		return dest;

	while (*dest != 0)
		dest++;

	while (*src != 0) {
		*dest = *src;
		dest++;
		src++;
	}

	*dest = 0;
	return dest;
}