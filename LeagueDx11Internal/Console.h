#pragma once

#pragma warning (disable:4996)

#include <Windows.h>
#include <string>
#include <unordered_set>
#include "XorString.h"
#include "lazy_importer.h"

#ifdef DEVELOPER
#define LOG(...) Console::Log(XorStr(__FUNCTION__), __LINE__, __VA_ARGS__)
#define LOGONCE(...) Console::LogOnce(XorStr(__FUNCTION__), __LINE__, __VA_ARGS__)
#else
#define LOG(...) Console::Log("",0,"")
#define LOGONCE(...) Console::Log("",0,"")

#endif

class Console
{
private:
#ifdef DEVELOPER

	inline static HANDLE hPipe;
	inline static DWORD dwWritten;
	inline static bool bInitialized = false;
	inline static const std::string sPipeName = XorStr("\\\\.\\pipe\\Pipe12");
	inline static std::unordered_set<std::string>printedLogs;

#endif

public:

	inline static bool bStopLog = true;

	static void Init()
	{
#ifdef DEVELOPER

		std::wstring wsPipeName(sPipeName.begin(), sPipeName.end());

		hPipe = LI_FN(CreateFileW).get()(wsPipeName.c_str(),
			/*GENERIC_READ | */GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (hPipe != INVALID_HANDLE_VALUE)
		{
			bInitialized = true;
			LOG(XorStr("Connected."));
		}
		else
		{
			std::string sError = XorStr("Client error: ") + std::to_string(GetLastError());
			LI_FN(MessageBoxA).get()(0, sError.c_str(), "", 0);
		}

#endif
	}

	static void Exit()
	{
#ifdef DEVELOPER

		if (!bInitialized)
			return;
		LOG(XorStr("Exitted"));
		LI_FN(CloseHandle).get()(hPipe);
		bInitialized = false;

#endif
	}

	static void Log(const std::string& function, const int& line, const char* format, ...)
	{
#ifdef DEVELOPER

		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		const std::string t = "[" + function + "|" + std::to_string(line) + "] " + buffer + "\n";
		va_end(args);

		Print(t);

#endif
	}

	static void LogOnce(const std::string& function, const int& line, const char* format, ...)
	{
#ifdef DEVELOPER

		const std::string temp = function + std::to_string(line);

		if (printedLogs.find(temp) != printedLogs.end())
			return;

		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		const std::string t = "[" + function + "|" + std::to_string(line) + "] " + buffer + "\n";
		va_end(args);

		Print(t);
		printedLogs.insert(temp);
	

#endif
	}

private:

	static void Print(const std::string& msg)
	{
#ifdef DEVELOPER

		if (!bInitialized || !bStopLog)
			return;

		LI_FN(WriteFile).get()(hPipe,
			msg.c_str(),
			msg.size(),
			&dwWritten,
			NULL);

#endif
	}
};
