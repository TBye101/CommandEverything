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
	register size_t i;
	register size_t length = Parsed->Words->size();

	//arg.append("\"");

	for (i = 0; i < length; i++)
	{

		if (i != 0)
		{
			arg.append(Parsed->Words->at(i));
		}
	}

	//arg.append("\"");

	if (Files->DoesDirectoryExist(&arg))
	{
		FilePath->append(Parsed->Words->at(1));
		FilePath->append("\\");
	}
	else
	{
		string invalid = "Invalid directory: ";
		invalid.append(arg);
		Console->WriteLine("Directory does not exist!");
		Console->WriteLine(&invalid);
	}
}

string* CommandCd::GetName()
{
	return new string("CD");
}

string* CommandCd::GetHelp()
{
	return new string("Changes the directory we are working from, just like in CMD. Cd \"..\" goes up to the parent directory, while cd \".\" goes to the current directory.");
}
