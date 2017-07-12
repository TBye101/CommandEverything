#include "stdafx.h"
#include "CommandExit.h"


CommandExit::CommandExit()
{
}


CommandExit::~CommandExit()
{
}

bool CommandExit::ShouldRunThisCommand(ParsedCommand *Parsed)
{
	return (Parsed->Words->at(0) == "exit");
}

void CommandExit::Run(ParsedCommand * Parsed)
{
	std::exit(0);
}

string* CommandExit::GetName()
{
	return new string("Exit");
}

string* CommandExit::GetHelp()
{
	return new string("Tells the console to release it's resources, and exit.");
}
