#pragma once

#include <Windows.h>

/// <summary>
/// Crashes the system.
/// </summary>
class CommandCrash : ICommand
{
public:
	CommandCrash();
	~CommandCrash();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};

