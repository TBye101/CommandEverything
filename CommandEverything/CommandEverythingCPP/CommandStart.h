#pragma once

/// <summary>
/// Used to start a new instance of our command line.
/// </summary>
class CommandStart : public ICommand
{
public:
	CommandStart();
	~CommandStart();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};