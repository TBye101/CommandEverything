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

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
};
