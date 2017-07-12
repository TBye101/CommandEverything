#include "stdafx.h"
#include "CommandStart.h"


CommandStart::CommandStart()
{
}


CommandStart::~CommandStart()
{
}

bool CommandStart::ShouldRunThisCommand(ParsedCommand *Parsed)
{
	return (Parsed->Words->at(0) == "start");
}

void CommandStart::Run(ParsedCommand * Parsed)
{
}

string * CommandStart::GetName()
{
	return new string("Start");
}

string * CommandStart::GetHelp()
{
	return new string("Starts a new instance of this command line.");
}
