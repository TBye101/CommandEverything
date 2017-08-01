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
	return (Parsed->Words->at(0) == "help");
}

void CommandHelp::Run(ParsedCommand * Parsed)
{
	string help = "";
	unsigned __int64 length = Commands->size();
	string* Name;
	string* HelpInfo;

	for (size_t i = 0; i < length; i++)
	{
		help = "";
		help.append(*Commands->at(i)->GetName());
		help.append(": ");
		help.append(*Commands->at(i)->GetHelp());
		Console->WriteLine(&help);

		cout << "\r\n";
		cout << "\r\n";
		Console->Log << "\r\n";
		Console->Log << "\r\n";
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