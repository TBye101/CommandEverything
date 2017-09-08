#pragma once

/// <summary>
/// Tells the program to cleanup, and exit.
/// </summary>
class CommandExit : public ICommand
{
public:
	CommandExit();
	~CommandExit();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};
