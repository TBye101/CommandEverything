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
	string* Test = Parsed->Words->at(0);
	Console->WriteLine(Test);
	string* name = new string("help");
	bool ret = (*Parsed->Words->at(0) == *name);
	Console->WriteLine(Parsed->Words->at(0));
	ToDelete->push_back(name);
	return ret;
}

void CommandHelp::Run(ParsedCommand * Parsed)
{
	string help = "";
	size_t length = Commands->size();
	for (size_t i = 0; i < length; i++)
	{
		help = "";
		help.append(*Commands->at(i)->GetName());
		help.append(": ");
		help.append(*Commands->at(i)->GetHelp());
		Console->WriteLine(&help);
	}
}

string * CommandHelp::GetName()
{
	return new string("Help");
}

string * CommandHelp::GetHelp()
{
	return new string("Shows some help information about all known commands.");
}
