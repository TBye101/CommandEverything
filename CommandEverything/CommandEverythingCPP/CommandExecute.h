#pragma once

/// <summary>
/// Used to open another program.
/// </summary>
class CommandExecute : public ICommand
{
public:
	CommandExecute();
	~CommandExecute();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};