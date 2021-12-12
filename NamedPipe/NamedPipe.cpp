#define PIPENAME "\\\\.\\pipe\\Pipe12"

#pragma warning(disable: 4996)

#include <iostream>
#include <Windows.h>
#include <time.h>

std::string GetCurrTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "<%d-%m-%Y %H:%M:%S>\n", timeinfo);
	return std::string(buffer);
}

void SetConsoleColor(int Color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, Color);
}

int main()
{
	HANDLE hPipe = CreateNamedPipeW(TEXT(PIPENAME), PIPE_ACCESS_INBOUND, PIPE_WAIT, 1, 0, 1024, 60 * 60 * 1000, NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Server Error: " << GetLastError() << std::endl;
		return 0;
	}

	std::cout << "Server started" << std::endl;

	char buffer[1024];
	DWORD dwRead;

	while (true)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				if (dwRead > 0)
				{
					buffer[dwRead] = '\0';
					SetConsoleColor(2);
					std::cout << GetCurrTime();
					SetConsoleColor(7);
					std::cout << buffer;
				}
			}
			DisconnectNamedPipe(hPipe);
		}
	}

	CloseHandle(hPipe);

	return 0;
}