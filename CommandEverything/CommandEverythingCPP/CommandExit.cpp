#include "stdafx.h"
#include "CommandExit.h"

CommandExit::CommandExit()
{
}

CommandExit::~CommandExit()
{
}

bool CommandExit::shouldRunThisCommand(ParsedCommand *Parsed)
{
	return (Parsed->Words->at(0) == "exit");
}

void CommandExit::run(ParsedCommand * Parsed)
{
	std::exit(0);
}

string* CommandExit::getName()
{
	return new string("Exit");
}

string* CommandExit::getHelp()
{
	return new string("Tells the console to release it's resources, and exit.");
}