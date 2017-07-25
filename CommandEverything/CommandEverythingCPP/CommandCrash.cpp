#include "stdafx.h"
#include "CommandCrash.h"


CommandCrash::CommandCrash()
{
}


CommandCrash::~CommandCrash()
{
}

bool CommandCrash::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "crash");
}

void CommandCrash::Run(ParsedCommand * Parsed)
{
	//BroadcastSystemMessage()
}

string* CommandCrash::GetName()
{
	return new string("Crash");
}

string* CommandCrash::GetHelp()
{
	return new string("Crashes the computer, and causes a BSOD (Blue Screen Of Death)");
}
