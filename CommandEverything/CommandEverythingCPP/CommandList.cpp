#include "stdafx.h"
#include "CommandList.h"


CommandList::CommandList()
{
}


CommandList::~CommandList()
{
}

bool CommandList::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "list");
}

void CommandList::Run(ParsedCommand* Parsed)
{
	if (FilePath->c_str() == "")
	{
		this->ListDriveLetters();
	}
	else
	{

	}
}

string* CommandList::GetName()
{
	return new string("List");
}

string* CommandList::GetHelp()
{
	return new string("Lists all availible options for the current state of the cd command.\r\n [Optional Flag] \"List directory\" will show only valid directories.");
}

void CommandList::ListDriveLetters()
{
}
