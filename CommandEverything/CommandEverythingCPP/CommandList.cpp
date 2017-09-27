#include "stdafx.h"
#include "CommandList.h"

CommandList::CommandList()
{
}

CommandList::~CommandList()
{
}

bool CommandList::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "list");
}

void CommandList::run(ParsedCommand* Parsed)
{
	bool ShowingFilesOnly = false;
	if (Parsed->Words->size() == 2 && Parsed->Words->at(1) == "files")
	{
		ShowingFilesOnly = true;
	}
	if (FilePath->empty())
	{
		this->listDriveLetters();
	}
	else
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(FilePath->c_str())) != NULL)
		{
			//print all the directories within directory
			if (ShowingFilesOnly)
			{
				while ((ent = readdir(dir)) != NULL)
				{
					//https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html has useful information on type flags.
					if (ent->d_type == DT_REG)
					{
						Console->WriteLine(ent->d_name);
					}
				}
				closedir(dir);
			}
			else
			{
				while ((ent = readdir(dir)) != NULL)
				{
					if (ent->d_type == DT_DIR)
					{
						Console->WriteLine(ent->d_name);
					}
				}
				closedir(dir);
			}
		}
		else
		{
			//could not open directory
			string err = "Could not open directory ";
			err.append("\"");
			err.append(*FilePath);
			err.append("\"");
			Console->WriteLine(&err);
		}
	}
}

string* CommandList::getName()
{
	return new string("List");
}

string* CommandList::getHelp()
{
	return new string("Lists all available options for the current state of the cd command.\r\n If you use \"List files\" you get all the files within the working directory.");
}

void CommandList::listDriveLetters()
{
	unsigned long mydrives = 100; // buffer length
	wchar_t lpBuffer[100]; // buffer for drive string storage
	unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer);
	__int64 drives = test / 4;
	string drive = "";
	char strbuffer[64];
	for (unsigned __int8 i = 0; i < drives; i++)
	{
		wctomb(strbuffer, lpBuffer[i * 4]);
		drive = strbuffer[0];
		drive.append(":/");
		Console->WriteLine(&drive);
	}
}