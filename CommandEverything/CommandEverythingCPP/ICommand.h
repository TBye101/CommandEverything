#pragma once

#include "ParsedCommand.h"

#include <string>

using namespace std;

class ICommand
{
public:
	
	virtual bool ShouldRunThisCommand(ParsedCommand* Parsed) = 0;
	virtual void Run(string* Input) = 0;
	virtual string* GetName() = 0;
	virtual string* GetHelp() = 0;
};