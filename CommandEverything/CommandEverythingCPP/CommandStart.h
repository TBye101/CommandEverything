#pragma once

/// <summary>
/// Used to start a new instance of our command line.
/// </summary>
class CommandStart : public ICommand
{
public:
	CommandStart();
	~CommandStart();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};