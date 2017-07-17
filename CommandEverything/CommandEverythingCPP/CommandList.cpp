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
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(FilePath->c_str())) != NULL)
		{
			//print all the files and directories within directory
			while ((ent = readdir(dir)) != NULL) 
			{
				Console->WriteLine(ent->d_name);
			}
			closedir(dir);
		}
		else
		{
			//could not open directory
			string err = "Could not open directory";
			err.append("\"");
			err.append(*FilePath);
			err.append("\"");
			Console->WriteLine(&err);
		}
		ToDelete->push_back(dir);
		ToDelete->push_back(ent);
	}
}

string* CommandList::GetName()
{
	return new string("List");
}

string* CommandList::GetHelp()
{
	return new string("Lists all availible options for the current state of the cd command.\r\n");
}

void CommandList::ListDriveLetters()
{
	register unsigned long mydrives = 100; // buffer length
	wchar_t lpBuffer[100]; // buffer for drive string storage
	unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer);

	string drive = "";
	char strbuffer[64];
	for (register unsigned __int8 i = 0; i < mydrives; i++)
	{
		drive = "";
		wctomb(strbuffer, lpBuffer[i]);
		drive.append(strbuffer);
		drive.append(":/");
		Console->WriteLine(&drive);
	}
}
