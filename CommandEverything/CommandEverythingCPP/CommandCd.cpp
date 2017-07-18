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
	if (Parsed->Words->size() < 2)
	{
		Console->WriteLine("Missing required argument.");
	}
	else
	{
		//If we need to go up a directory.
		if (Parsed->Words->at(1) == "..")
		{
			this->GotoParentDir();
			Console->WriteLine(FilePath);
			return;
		}

		//If we need to stay at the same one.
		if (Parsed->Words->at(1) == ".")
		{
			return;
		}

		//If we want to go deeper into a subdirectory.
		this->CdNormal(Parsed);

		Console->WriteLine(FilePath);
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

void CommandCd::GotoParentDir()
{
	bool Slashfound = false;

	register size_t i = FilePath->size();

	while (i != 0)
	{
		if (FilePath->at(i) == '\\')
		{
			if (!Slashfound)
			{
				//Found the first slash
				Slashfound = true;
			}
			else
			{
				//Found the second slash, calculating how many characters to remove from the end of the path string.
				size_t pop = FilePath->size() - i;
				--pop;

				while (pop != 0)
				{
					FilePath->pop_back();
					--pop;
				}
				break;
			}
		}
		--i;
	}
}

void CommandCd::CdNormal(ParsedCommand* Parsed)
{
	string arg = *FilePath;
	register size_t i;
	register size_t length = Parsed->Words->size();

	for (i = 0; i < length; i++)
	{

		if (i != 0)
		{
			arg.append(Parsed->Words->at(i));
		}
	}

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
