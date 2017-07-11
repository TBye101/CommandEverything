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
	
	bool ShouldRunThisCommand(ParsedCommand* Parsed);
	void Run(ParsedCommand* Parsed);
	string* GetName();
	string* GetHelp();
};

