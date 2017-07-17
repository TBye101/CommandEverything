#include "stdafx.h"
#include "CommandCd.h"


CommandCd::CommandCd()
{
}


CommandCd::~CommandCd()
{
}

bool CommandCd::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "cd");
}

void CommandCd::Run(ParsedCommand* Parsed)
{
	string arg = *FilePath;
	arg.append(Parsed->Words->at(1));

	if (Files->DoesDirectoryExist(&arg))
	{
		FilePath->append(arg);
		FilePath->append("/");
	}
	else
	{
		Console->WriteLine("Directory does not exist!");
	}
}

string* CommandCd::GetName()
{
	return new string("CD");
}

string* CommandCd::GetHelp()
{
	return new string("Changes the directory we are working from, just like in CMD.");
}
