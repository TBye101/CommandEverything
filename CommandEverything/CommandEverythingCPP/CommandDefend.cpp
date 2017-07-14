#include "stdafx.h"
#include "CommandDefend.h"


CommandDefend::CommandDefend()
{
}


CommandDefend::~CommandDefend()
{
}

bool CommandDefend::ShouldRunThisCommand(ParsedCommand * Parsed)
{
	return false;
}

void CommandDefend::Run(ParsedCommand * Parsed)
{
}

string * CommandDefend::GetName()
{
	return nullptr;
}

string * CommandDefend::GetHelp()
{
	return nullptr;
}
