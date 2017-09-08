#pragma once

#include <algorithm>

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
