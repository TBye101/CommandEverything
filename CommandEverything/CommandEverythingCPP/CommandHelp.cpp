#include "stdafx.h"
#include "CommandHelp.h"


CommandHelp::CommandHelp()
{
}


CommandHelp::~CommandHelp()
{
}

bool CommandHelp::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return ( && Parsed->Words[0] == "help");
}

void CommandHelp::Run(ParsedCommand * Parsed)
{
}

string * CommandHelp::GetName()
{
	return nullptr;
}

string * CommandHelp::GetHelp()
{
	return nullptr;
}