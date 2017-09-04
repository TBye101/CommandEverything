#include "stdafx.h"
#include "CommandDelete.h"

CommandDelete::CommandDelete()
{
}

CommandDelete::~CommandDelete()
{
}

bool CommandDelete::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "delete");
}

void CommandDelete::Run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() < 2)
	{
		Console->WriteLine("Missing argument!");
	}
	else
	{
		wchar_t path[MAX_PATH];
		size_t size = strlen(Parsed->Words->at(1).c_str());
		size_t outSize;
		mbstowcs_s(&outSize, path, Parsed->Words->at(1).c_str(), size);

		if (!DeleteFile(path))
		{
			Console->WriteLine("Failed to delete a file!");
		}
	}
}

string* CommandDelete::GetName()
{
	return new string("Delete");
}

string* CommandDelete::GetHelp()
{
	return new string("Deletes the specified file. The second argument should be a file path.");
}