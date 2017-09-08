#pragma once

/// <summary>
/// Deletes a file.
/// </summary>
class CommandDelete : public ICommand
{
public:
	CommandDelete();
	~CommandDelete();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};
