#pragma once

/// <summary>
/// Used to open another program.
/// </summary>
class CommandExecute : public ICommand
{
public:
	CommandExecute();
	~CommandExecute();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};