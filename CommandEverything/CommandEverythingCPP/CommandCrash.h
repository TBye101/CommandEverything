#pragma once

#include <Windows.h>
#include <thread>
#include <processthreadsapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>
#include <algorithm>

/// <summary>
/// Crashes the system.
/// </summary>
class CommandCrash : public ICommand
{
public:
	CommandCrash();
	~CommandCrash();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};

