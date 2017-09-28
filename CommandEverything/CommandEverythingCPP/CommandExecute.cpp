#include "stdafx.h"
#include "CommandExecute.h"

CommandExecute::CommandExecute()
{
}

CommandExecute::~CommandExecute()
{
}

bool CommandExecute::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->words->at(0) == "execute");
}

void CommandExecute::run(ParsedCommand* Parsed)
{
	if (Parsed->words->size() < 2 || Parsed->words->at(1) == "same" && Parsed->words->size() < 3)
	{
		Console->WriteLine("Invalid argument(s)");
	}
	else
	{
		wchar_t path[MAX_PATH];
		size_t size;
		size_t outSize;
		STARTUPINFO info = { sizeof(info) };
		PROCESS_INFORMATION processInfo;
		bool success = false;

		//If we want to open the process in this console window or not.
		if (Parsed->words->at(1) == "same")
		{
			size = strlen(Parsed->words->at(2).c_str());;
			mbstowcs_s(&outSize, path, Parsed->words->at(2).c_str(), size);
			success = CreateProcess(path, NULL, NULL, NULL, true, 0, NULL, NULL, &info, &processInfo);
		}
		else
		{
			size = strlen(Parsed->words->at(1).c_str());
			mbstowcs_s(&outSize, path, Parsed->words->at(1).c_str(), size);
			success = CreateProcess(path, NULL, NULL, NULL, true, CREATE_NEW_CONSOLE, NULL, NULL, &info, &processInfo);
		}

		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);

		if (!success)
		{
			Console->WriteLine("Failed to launch an application.");
		}
	}
}

string* CommandExecute::getName()
{
	return new string("Execute");
}

string* CommandExecute::getHelp()
{
	return new string("Launches the specified process.\r\n \"Execute same\" will open the process in the same console window as this one, if possible. You must specify a filepath including the filename and extension in the second or third word.");
}