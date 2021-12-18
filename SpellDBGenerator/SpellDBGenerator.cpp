#include <iostream>
#include <string>
#include <regex>

#include "HTTP.h"
#include "json.h"

using namespace nlohmann;

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
	return str;
}

int main()
{
	// get current patch
	std::string getVersions = http->Request("GET", "https://ddragon.leagueoflegends.com/api/versions.json");
	std::string version = json::parse(getVersions)[0];

	std::cout << "// " + version << std::endl;

	// get list of champions
	std::vector<std::string>champions;
	std::string getChampions = http->Request("GET", "http://ddragon.leagueoflegends.com/cdn/" + version + "/data/en_US/champion.json");
	json j = json::parse(getChampions)["data"];
	for (json::iterator it = j.begin(); it != j.end(); it++)
	{
		champions.emplace_back(ToLower(it.key()));
	}

	// get champion links
	for (auto& champ : champions)
	{
		std::string url = "https://raw.communitydragon.org/latest/game/data/characters/"
			+ champ + "/" + champ + ".bin.json";
		std::string getData = http->Request("GET", url);
		std::cout << url << std::endl;

		json j = json::parse(getData);
		for (json::iterator it = j.begin(); it != j.end(); it++)
		{
			if (!it.key().contains("Spells/"))
				continue;

			if (!it.value()["mScriptName"].is_string())
				continue;

			if (it.value().find("mSpell") == it.value().end())
				continue;

			std::string mScriptName = it.value()["mScriptName"];

			// for testing
			if (mScriptName != "ThreshQ")
				continue;

			std::cout << mScriptName << std::endl;

			auto root = it.value()["mSpell"];

			if (root.find("mCastTime") != root.end())
			{
				float mCastTime = root["mCastTime"];
				std::cout << "mCastTime " << mCastTime << std::endl;
			}
			if (root.find("castRange") != root.end())
			{
				int castRange = root["castRange"][0];
				if (root.find("castConeDistance") != root.end())
					castRange += root["castConeDistance"];
				std::cout << "castRange " << castRange << std::endl;
			}

			if (root.find("missileSpeed") != root.end())
			{
				float missileSpeed = root["missileSpeed"];
				std::cout << "missileSpeed " << missileSpeed << std::endl;
			}
		}
	}

	return 0;
}