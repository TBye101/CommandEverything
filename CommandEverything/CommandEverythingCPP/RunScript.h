#pragma once

#include "ParsedCommand.h"
#include "ICommand.h"

#include <algorithm>
#include <string>

using namespace std;

/// <summary>
/// Runs a script.
/// </summary>
class RunScript : public ICommand
{
public:
	RunScript();
	~RunScript();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};
