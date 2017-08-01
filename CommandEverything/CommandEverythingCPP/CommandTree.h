#pragma once

#include "ICommand.h"

/// <summary>
/// Shows a tree structure of where the current working directory is.
/// </summary>
class CommandTree : public ICommand
{
public:
	CommandTree();
	~CommandTree();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};

