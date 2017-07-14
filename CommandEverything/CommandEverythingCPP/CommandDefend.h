#pragma once

/// <summary>
/// Defends against new programs being launched.
/// </summary>
class CommandDefend : public ICommand
{
public:
	CommandDefend();
	~CommandDefend();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};