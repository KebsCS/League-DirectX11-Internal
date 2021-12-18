#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <Windows.h>
#include <format>
#include <string>
#include <filesystem>
#include <regex>
#include <assert.h>

#pragma warning (disable:4996)

#include "HTTP.h"

// stb_compress* from stb.h - declaration
typedef unsigned int stb_uint;
typedef unsigned char stb_uchar;
stb_uint stb_compress(stb_uchar* out, stb_uchar* in, stb_uint len);

bool bnoCircles = true;
bool bnoTFT = true;
bool bnoMapAssets = true;
bool bnoPets = true;

// save arrays in 1 line or multiple lines
bool bOneLine = true;
bool bOnlyPNG = true;

void DownloadAllIcons()
{
	std::string get = http->Request("GET", "https://raw.communitydragon.org/latest/game/assets/characters/");

	const std::regex regex(R"(<a href="[\w\.]+/?" title="[\w\.]+">([\w\.]+)/?</a>)");
	std::smatch champsMatch;

	while (std::regex_search(get, champsMatch, regex))
	{
		std::string champName = champsMatch[1].str();

		get = champsMatch.suffix();

		if (bnoTFT && champName.find("tft") != std::string::npos)
			continue;
		if (bnoMapAssets && ((champName.find("sru_") != std::string::npos)
			|| (champName.find("ha_") != std::string::npos)
			|| (champName.find("bw_") != std::string::npos)
			|| (champName.find("nexusblitz") != std::string::npos)
			|| (champName.find("sruap_") != std::string::npos)
			|| (champName.find("srx_") != std::string::npos)
			|| (champName.find("chaos") != std::string::npos)
			|| (champName.find("order") != std::string::npos)
			|| (champName.find("doombots") != std::string::npos)
			|| (champName.find("domination") != std::string::npos)
			|| (champName.find("tutorial") != std::string::npos)
			|| (champName.find("slime_") != std::string::npos)
			|| (champName.find("npc_") != std::string::npos)
			|| (champName.find("kingporo") != std::string::npos)
			|| (champName.find("testcube") != std::string::npos)))
			continue;
		if (bnoPets && champName.find("pet") != std::string::npos)
			continue;

		std::cout << champName << std::endl;

		std::string url = "https://raw.communitydragon.org/latest/game/assets/characters/" + champName + "/hud/";
		std::string getHud = http->Request("GET", url);
		std::smatch portraitMatch;
		while (std::regex_search(getHud, portraitMatch, regex))
		{
			getHud = portraitMatch.suffix();
			std::string portraitName = portraitMatch[1].str();
			std::string portraitUrl = url + portraitName;
			if (portraitName.find(".png") != std::string::npos)
			{
				if (bnoCircles && portraitName.find("circle") != std::string::npos)
					continue;

				http->DownloadFile(portraitName, "", url);
			}
		}

		url = "https://raw.communitydragon.org/latest/game/assets/characters/" + champName + "/hud/icons2d/";
		std::string getIcons2d = http->Request("GET", url);
		while (std::regex_search(getIcons2d, portraitMatch, regex))
		{
			getIcons2d = portraitMatch.suffix();
			std::string portraitName = portraitMatch[1].str();
			std::string portraitUrl = url + portraitName;
			if (portraitName.find(".png") != std::string::npos)
			{
				http->DownloadFile(portraitName, "", url);
			}
		}
	}

	// summoner spells
	std::string url = "https://raw.communitydragon.org/latest/game/data/spells/icons2d/";
	std::string getSumms = http->Request("GET", url);
	std::smatch summsMatch;
	while (std::regex_search(getSumms, summsMatch, regex))
	{
		getSumms = summsMatch.suffix();
		std::string portraitName = summsMatch[1].str();
		std::string portraitUrl = url + portraitName;
		if (portraitName.find("summoner") == std::string::npos)
			continue;

		if (portraitName.find(".png") != std::string::npos)
		{
			http->DownloadFile(portraitName, "", url);
		}
	}
}

int main(int argc, char** argv)
{
	//DownloadAllIcons();

	// output filename
	std::string file = "Images.h";
	std::ofstream outFile(file);

	outFile << "#pragma once\n\nnamespace Images\n{\n\n";

	for (const auto& inputFile : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		std::string inputFileName = inputFile.path().filename().string();

		// ignore itself and output file
		if ((inputFile.path() == argv[0]) || file == inputFileName)
			continue;

		// todo get name from argv[0]
		if (inputFileName.find("LolImagesTo") != std::string::npos)
			continue;

		// array names cant start with a digit
		if (std::isdigit(inputFileName[0]))
			continue;

		if (inputFileName.find(".png") == std::string::npos)
			continue;

		// Read file
		FILE* f = fopen(inputFileName.c_str(), "rb");
		if (!f) return false;
		int data_sz;
		if (fseek(f, 0, SEEK_END) || (data_sz = (int)ftell(f)) == -1 || fseek(f, 0, SEEK_SET)) { fclose(f); return false; }
		char* data = new char[data_sz + 4];
		if (fread(data, 1, data_sz, f) != (size_t)data_sz) { fclose(f); delete[] data; return false; }
		memset((void*)(((char*)data) + data_sz), 0, 4);
		fclose(f);

		// Compress
		int maxlen = data_sz + 512 + (data_sz >> 2) + sizeof(int); // total guess
		char* compressed = new char[maxlen];
		int compressed_sz = stb_compress((stb_uchar*)compressed, (stb_uchar*)data, data_sz);
		memset(compressed + compressed_sz, 0, maxlen - compressed_sz);

		std::string arrayName = inputFileName.substr(0, inputFileName.find("."));

		outFile << "static const unsigned int " << arrayName << "[" << (int)((compressed_sz + 3) / 4) * 4 << "/4] = {";
		int column = 0;
		for (int i = 0; i < compressed_sz; i += 4)
		{
			unsigned int d = *(unsigned int*)(compressed + i);
			if (!bOneLine && (column % 12 == 0))
			{
				outFile << "\n\t";
			}

			column++;
			char out[255];
			sprintf(out, "0x%08x, ", d);
			outFile << out;
		}
		outFile << "};\n\n";

		std::cout << "Converted " << inputFileName << std::endl;
	}

	outFile << "\n}\n";
}

// stb_compress* from stb.h - definition

////////////////////           compressor         ///////////////////////

static stb_uint stb_adler32(stb_uint adler32, stb_uchar* buffer, stb_uint buflen)
{
	const unsigned long ADLER_MOD = 65521;
	unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
	unsigned long blocklen, i;

	blocklen = buflen % 5552;
	while (buflen) {
		for (i = 0; i + 7 < blocklen; i += 8) {
			s1 += buffer[0], s2 += s1;
			s1 += buffer[1], s2 += s1;
			s1 += buffer[2], s2 += s1;
			s1 += buffer[3], s2 += s1;
			s1 += buffer[4], s2 += s1;
			s1 += buffer[5], s2 += s1;
			s1 += buffer[6], s2 += s1;
			s1 += buffer[7], s2 += s1;

			buffer += 8;
		}

		for (; i < blocklen; ++i)
			s1 += *buffer++, s2 += s1;

		s1 %= ADLER_MOD, s2 %= ADLER_MOD;
		buflen -= blocklen;
		blocklen = 5552;
	}
	return (s2 << 16) + s1;
}

static unsigned int stb_matchlen(stb_uchar* m1, stb_uchar* m2, stb_uint maxlen)
{
	stb_uint i;
	for (i = 0; i < maxlen; ++i)
		if (m1[i] != m2[i]) return i;
	return i;
}

// simple implementation that just takes the source data in a big block

static stb_uchar* stb__out;
static FILE* stb__outfile;
static stb_uint   stb__outbytes;

static void stb__write(unsigned char v)
{
	fputc(v, stb__outfile);
	++stb__outbytes;
}

//#define stb_out(v)    (stb__out ? *stb__out++ = (stb_uchar) (v) : stb__write((stb_uchar) (v)))
#define stb_out(v)    do { if (stb__out) *stb__out++ = (stb_uchar) (v); else stb__write((stb_uchar) (v)); } while (0)

static void stb_out2(stb_uint v) { stb_out(v >> 8); stb_out(v); }
static void stb_out3(stb_uint v) { stb_out(v >> 16); stb_out(v >> 8); stb_out(v); }
static void stb_out4(stb_uint v) { stb_out(v >> 24); stb_out(v >> 16); stb_out(v >> 8); stb_out(v); }

static void outliterals(stb_uchar* in, int numlit)
{
	while (numlit > 65536) {
		outliterals(in, 65536);
		in += 65536;
		numlit -= 65536;
	}

	if (numlit == 0);
	else if (numlit <= 32)    stb_out(0x000020 + numlit - 1);
	else if (numlit <= 2048)    stb_out2(0x000800 + numlit - 1);
	else /*  numlit <= 65536) */ stb_out3(0x070000 + numlit - 1);

	if (stb__out) {
		memcpy(stb__out, in, numlit);
		stb__out += numlit;
	}
	else
		fwrite(in, 1, numlit, stb__outfile);
}

static int stb__window = 0x40000; // 256K

static int stb_not_crap(int best, int dist)
{
	return   ((best > 2 && dist <= 0x00100)
		|| (best > 5 && dist <= 0x04000)
		|| (best > 7 && dist <= 0x80000));
}

static  stb_uint stb__hashsize = 32768;

// note that you can play with the hashing functions all you
// want without needing to change the decompressor
#define stb__hc(q,h,c)      (((h) << 7) + ((h) >> 25) + q[c])
#define stb__hc2(q,h,c,d)   (((h) << 14) + ((h) >> 18) + (q[c] << 7) + q[d])
#define stb__hc3(q,c,d,e)   ((q[c] << 14) + (q[d] << 7) + q[e])

static unsigned int stb__running_adler;

static int stb_compress_chunk(stb_uchar* history,
	stb_uchar* start,
	stb_uchar* end,
	int length,
	int* pending_literals,
	stb_uchar** chash,
	stb_uint mask)
{
	(void)history;
	int window = stb__window;
	stb_uint match_max;
	stb_uchar* lit_start = start - *pending_literals;
	stb_uchar* q = start;

#define STB__SCRAMBLE(h)   (((h) + ((h) >> 16)) & mask)

	// stop short of the end so we don't scan off the end doing
	// the hashing; this means we won't compress the last few bytes
	// unless they were part of something longer
	while (q < start + length && q + 12 < end) {
		int m;
		stb_uint h1, h2, h3, h4, h;
		stb_uchar* t;
		int best = 2, dist = 0;

		if (q + 65536 > end)
			match_max = end - q;
		else
			match_max = 65536;

#define stb__nc(b,d)  ((d) <= window && ((b) > 9 || stb_not_crap(b,d)))

#define STB__TRY(t,p)  /* avoid retrying a match we already tried */ \
    if (p ? dist != q-t : 1)                             \
    if ((m = stb_matchlen(t, q, match_max)) > best)     \
    if (stb__nc(m,q-(t)))                                \
    best = m, dist = q - (t)

		// rather than search for all matches, only try 4 candidate locations,
		// chosen based on 4 different hash functions of different lengths.
		// this strategy is inspired by LZO; hashing is unrolled here using the
		// 'hc' macro
		h = stb__hc3(q, 0, 1, 2); h1 = STB__SCRAMBLE(h);
		t = chash[h1]; if (t) STB__TRY(t, 0);
		h = stb__hc2(q, h, 3, 4); h2 = STB__SCRAMBLE(h);
		h = stb__hc2(q, h, 5, 6);        t = chash[h2]; if (t) STB__TRY(t, 1);
		h = stb__hc2(q, h, 7, 8); h3 = STB__SCRAMBLE(h);
		h = stb__hc2(q, h, 9, 10);        t = chash[h3]; if (t) STB__TRY(t, 1);
		h = stb__hc2(q, h, 11, 12); h4 = STB__SCRAMBLE(h);
		t = chash[h4]; if (t) STB__TRY(t, 1);

		// because we use a shared hash table, can only update it
		// _after_ we've probed all of them
		chash[h1] = chash[h2] = chash[h3] = chash[h4] = q;

		if (best > 2)
			assert(dist > 0);

		// see if our best match qualifies
		if (best < 3) { // fast path literals
			++q;
		}
		else if (best > 2 && best <= 0x80 && dist <= 0x100) {
			outliterals(lit_start, q - lit_start); lit_start = (q += best);
			stb_out(0x80 + best - 1);
			stb_out(dist - 1);
		}
		else if (best > 5 && best <= 0x100 && dist <= 0x4000) {
			outliterals(lit_start, q - lit_start); lit_start = (q += best);
			stb_out2(0x4000 + dist - 1);
			stb_out(best - 1);
		}
		else if (best > 7 && best <= 0x100 && dist <= 0x80000) {
			outliterals(lit_start, q - lit_start); lit_start = (q += best);
			stb_out3(0x180000 + dist - 1);
			stb_out(best - 1);
		}
		else if (best > 8 && best <= 0x10000 && dist <= 0x80000) {
			outliterals(lit_start, q - lit_start); lit_start = (q += best);
			stb_out3(0x100000 + dist - 1);
			stb_out2(best - 1);
		}
		else if (best > 9 && dist <= 0x1000000) {
			if (best > 65536) best = 65536;
			outliterals(lit_start, q - lit_start); lit_start = (q += best);
			if (best <= 0x100) {
				stb_out(0x06);
				stb_out3(dist - 1);
				stb_out(best - 1);
			}
			else {
				stb_out(0x04);
				stb_out3(dist - 1);
				stb_out2(best - 1);
			}
		}
		else {  // fallback literals if no match was a balanced tradeoff
			++q;
		}
	}

	// if we didn't get all the way, add the rest to literals
	if (q - start < length)
		q = start + length;

	// the literals are everything from lit_start to q
	*pending_literals = (q - lit_start);

	stb__running_adler = stb_adler32(stb__running_adler, start, q - start);
	return q - start;
}

static int stb_compress_inner(stb_uchar* input, stb_uint length)
{
	int literals = 0;
	stb_uint len, i;

	stb_uchar** chash;
	chash = (stb_uchar**)malloc(stb__hashsize * sizeof(stb_uchar*));
	if (chash == NULL) return 0; // failure
	for (i = 0; i < stb__hashsize; ++i)
		chash[i] = NULL;

	// stream signature
	stb_out(0x57); stb_out(0xbc);
	stb_out2(0);

	stb_out4(0);       // 64-bit length requires 32-bit leading 0
	stb_out4(length);
	stb_out4(stb__window);

	stb__running_adler = 1;

	len = stb_compress_chunk(input, input, input + length, length, &literals, chash, stb__hashsize - 1);
	assert(len == length);

	outliterals(input + length - literals, literals);

	free(chash);

	stb_out2(0x05fa); // end opcode

	stb_out4(stb__running_adler);

	return 1; // success
}

stb_uint stb_compress(stb_uchar* out, stb_uchar* input, stb_uint length)
{
	stb__out = out;
	stb__outfile = NULL;

	stb_compress_inner(input, length);

	return stb__out - out;
}