#pragma once

#include <string>

using namespace std;

class ICommand
{
public:
	
	virtual bool ShouldRunThisCommand(string* Input) = 0;
	virtual void Run(string* Input) = 0;
	virtual string* GetName() = 0;
	virtual string* GetHelp() = 0;
};