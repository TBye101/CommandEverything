#pragma once
#include <stdio.h>
#include "Header Only\dirent.h"

/// <summary>
/// Lists all availible options for the Cd command.
/// </summary>
class CommandList : public ICommand
{
public:
	CommandList();
	~CommandList();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();

private:
	/// <summary>
	/// Lists all valid drive letters.
	/// </summary>
	void ListDriveLetters();
};