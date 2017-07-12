#pragma once

/// <summary>
/// Tells the program to cleanup, and exit.
/// </summary>
class CommandExit : public ICommand
{
public:
	CommandExit();
	~CommandExit();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};

