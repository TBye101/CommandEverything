#pragma once

#include <algorithm>
#include <Psapi.h>

#include "ICommand.h"

class CommandKill : public ICommand
{
public:
	CommandKill();
	~CommandKill();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};