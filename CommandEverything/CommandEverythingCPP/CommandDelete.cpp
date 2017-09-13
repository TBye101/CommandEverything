#include "stdafx.h"
#include "CommandDelete.h"

CommandDelete::CommandDelete()
{
}

CommandDelete::~CommandDelete()
{
}

bool CommandDelete::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "delete");
}

void CommandDelete::run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() < 2)
	{
		Console->WriteLine("Missing argument!");
	}
	else
	{
		string pat = *FilePath;
		register unsigned __int64 length = Input.size();

		for (register unsigned __int64 i = 0; i < length; i++)
		{
			if (Input[i] == 'd' || Input[i] == 'D')
			{
				pat.append(Input.substr(i + 7));
				break;
			}
		}

		wchar_t path[MAX_PATH];
		const char* pat_c = pat.c_str();
		unsigned __int64 size = strlen(pat_c);
		unsigned __int64 outSize;
		mbstowcs_s(&outSize, path, pat_c, size);

		if (!DeleteFile(path))
		{
			Console->WriteLine("Failed to delete a file!");
		}
	}
}

string* CommandDelete::getName()
{
	return new string("Delete");
}

string* CommandDelete::getHelp()
{
	return new string("Deletes the specified file. The second argument should be a file path.");
}