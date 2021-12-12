#pragma once

class BuffManager
{
public:
	DWORD test;

	/*std::vector<DWORD> GetBuffList()
	{
		std::vector<DWORD> buffs;
		DWORD buffBegin = *reinterpret_cast<DWORD*>((DWORD)this + 0x10);
		DWORD buffEnd = *reinterpret_cast<DWORD*>((DWORD)this + 0x14);

		if (buffBegin != 0 && buffEnd != 0)
		{
			for (DWORD i = 0; i < (buffEnd - buffBegin); i += 0x08)
			{
				const DWORD buff = *reinterpret_cast<DWORD*>(buffBegin + i);
				if (buff != 0)
					buffs.emplace_back(buff);
			}
		}

		return buffs;
	}*/

	// todo, crash
	std::vector<std::string> GetBuffList()
	{
		std::vector<std::string> buffs;
		DWORD buffBegin = *reinterpret_cast<DWORD*>((DWORD)this + 0x10);
		DWORD buffEnd = *reinterpret_cast<DWORD*>((DWORD)this + 0x14);

		if (buffBegin != 0 && buffEnd != 0)
		{
			for (DWORD i = 0; i < (buffEnd - buffBegin); i += 0x08)
			{
				const DWORD buff = *reinterpret_cast<DWORD*>(buffBegin + i);
				if (buff < 0x1000)
					continue;
				/*const DWORD buffInfo = *reinterpret_cast<DWORD*>(buff + 0x8);
				if (buffInfo  < 0x1000)
					continue;

				char* buffname = reinterpret_cast<char*>(buffInfo + 0x8);
				*/
				buffs.emplace_back(std::to_string(buff));
			}
		}

		return buffs;
	}
};