#pragma once

/// <summary>
/// Lists all availible options for the Cd command.
/// </summary>
class CommandList : public ICommand
{
public:
	CommandList();
	~CommandList();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();

private:
	/// <summary>
	/// Lists all valid drive letters.
	/// </summary>
	void ListDriveLetters();
};

