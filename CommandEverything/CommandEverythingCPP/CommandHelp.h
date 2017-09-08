#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <algorithm>

using namespace std;

/// <summary>
/// Shows some help information about commands.
/// </summary>
class CommandHelp : public ICommand
{
public:
	CommandHelp();
	~CommandHelp();

	bool shouldRunThisCommand(ParsedCommand* Parsed);
	void run(ParsedCommand* Parsed);
	string* getName();
	string* getHelp();
};
