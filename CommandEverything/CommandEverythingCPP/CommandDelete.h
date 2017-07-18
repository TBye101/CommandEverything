#pragma once

/// <summary>
/// Deletes a file.
/// </summary>
class CommandDelete : public ICommand
{
public:
	CommandDelete();
	~CommandDelete();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};

