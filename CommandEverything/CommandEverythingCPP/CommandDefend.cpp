#include "stdafx.h"
#include "CommandDefend.h"


CommandDefend::CommandDefend()
{
}


CommandDefend::~CommandDefend()
{
}

bool CommandDefend::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "defend");
}

void CommandDefend::Run(ParsedCommand* Parsed)
{
	//There wasn't another word to determine whether we were to start or stop
	if (Parsed->Words->size() < 2)
	{
		string error = "Missing required command argument.";
		Console->WriteLine(&error);
		return;
	}

	if (Parsed->Words->at(1) == "start")
	{
		this->DefendStart();
		return;
	}

	if (Parsed->Words->at(1) == "stop")
	{
		this->DefendStop();
		return;
	}

	string error = "Invalid command argument";
	Console->WriteLine(&error);
}

string* CommandDefend::GetName()
{
	return new string("Defend");
}

string* CommandDefend::GetHelp()
{
	return new string("Kills all applications that were not running before Defend was started.\r\n Usage: Defend Start to start Defend, Defend Stop to stop Defend.");
}

void CommandDefend::DefendStart()
{
	typedef void (CommandDefend::*funcptr)();
	funcptr ptr = &CommandDefend::DefendStart;
	thread neW(&this->DefendStart());
}

void CommandDefend::DefendStop()
{
}

void CommandDefend::Defend()
{
}
