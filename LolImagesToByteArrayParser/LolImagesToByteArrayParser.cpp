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

#include "HTTP.h"

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
}

int main(int argc, char** argv)
{
	DownloadAllIcons();

	// output filename
	std::string file = "out.txt";
	std::ofstream outFile(file);

	outFile << "#pragma once\n\nnamespace Images\n{\n\n";

	for (const auto& inputFile : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		std::string inputFileName = inputFile.path().filename().string();

		// ignore itself and output file
		if ((inputFile.path() == argv[0]) || file == inputFileName)
			continue;

		// array names cant start with a digit
		if (std::isdigit(inputFileName[0]))
			continue;

		if (inputFileName.find(".png") == std::string::npos)
			continue;

		std::ifstream inFile(inputFileName, std::ios_base::binary);

		inFile.seekg(0, std::ios_base::end);
		size_t length = inFile.tellg();
		inFile.seekg(0, std::ios_base::beg);

		std::vector<char> buffer;
		buffer.reserve(length);
		std::copy(std::istreambuf_iterator<char>(inFile),
			std::istreambuf_iterator<char>(),
			std::back_inserter(buffer));

		std::string arrayName = inputFileName.substr(0, inputFileName.find("."));

		outFile << "static const unsigned char " << arrayName << "[] = {";
		int i = 0;
		for (unsigned char c : buffer)
		{
			if (!bOneLine && (i % 12 == 0))
			{
				outFile << "\n\t";
			}

			i++;
			outFile << std::format("{:#04x}, ", c);
		}
		outFile << "};\n\n";

		std::cout << "Converted " << inputFileName << std::endl;
	}

	outFile << "\n}\n";
}