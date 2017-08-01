#pragma once

#include <algorithm>
#include <Psapi.h>

#include "ICommand.h"

class CommandKill : public ICommand
{
public:
	CommandKill();
	~CommandKill();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
private:

};

