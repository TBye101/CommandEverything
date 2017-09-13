#include "stdafx.h"
#include "CommandHelp.h"

CommandHelp::CommandHelp()
{
}

CommandHelp::~CommandHelp()
{
}

bool CommandHelp::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "help");
}

void CommandHelp::run(ParsedCommand * Parsed)
{
	string help = "";
	unsigned __int64 length = Commands->size();
	string* Name;
	string* HelpInfo;

	for (register unsigned __int64 i = 0; i < length; i++)
	{
		Name = Commands->at(i)->getName();
		HelpInfo = Commands->at(i)->getHelp();
		help = "";
		help.append(*Name);
		help.append(": ");
		help.append(*HelpInfo);
		Console->writeLine(&help);

		cout << "\r\n";
		cout << "\r\n";
		Console->Log << "\r\n";
		Console->Log << "\r\n";

		delete Name;
		delete HelpInfo;
	}
}

string* CommandHelp::getName()
{
	return new string("Help");
}

string* CommandHelp::getHelp()
{
	return new string("Shows some help information about all known commands.");
}