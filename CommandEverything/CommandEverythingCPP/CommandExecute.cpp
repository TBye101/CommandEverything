#include "stdafx.h"
#include "CommandExecute.h"

CommandExecute::CommandExecute()
{
}

CommandExecute::~CommandExecute()
{
}

bool CommandExecute::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "execute");
}

void CommandExecute::Run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() < 2 || Parsed->Words->at(1) == "same" && Parsed->Words->size() < 3)
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
		if (Parsed->Words->at(1) == "same")
		{
			size = strlen(Parsed->Words->at(2).c_str());;
			mbstowcs_s(&outSize, path, Parsed->Words->at(2).c_str(), size);
			success = CreateProcess(path, NULL, NULL, NULL, true, 0, NULL, NULL, &info, &processInfo);
		}
		else
		{
			size = strlen(Parsed->Words->at(1).c_str());
			mbstowcs_s(&outSize, path, Parsed->Words->at(1).c_str(), size);
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

string* CommandExecute::GetName()
{
	return new string("Execute");
}

string* CommandExecute::GetHelp()
{
	return new string("Launches the specified process.\r\n \"Execute same\" will open the process in the same console window as this one, if possible. You must specify a filepath including the filename and extension in the second or third word.");
}