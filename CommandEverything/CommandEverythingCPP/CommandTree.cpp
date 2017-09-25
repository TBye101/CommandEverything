#include "stdafx.h"
#include "CommandTree.h"

CommandTree::CommandTree()
{
}

CommandTree::~CommandTree()
{
}

bool CommandTree::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "tree");
}

void CommandTree::run(ParsedCommand* Parsed)
{
	clock_t start;
	double duration;
	start = clock();

	//If no working directory, do all drives!
	if (*FilePath == "")
	{
		register unsigned long mydrives = 100; // buffer length
		register wchar_t lpBuffer[100]; // buffer for drive string storage
		register unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer);
		register unsigned __int32 drives = test / 4;
		string drive = "";
		char strbuffer[64];

		for (register unsigned __int8 i = 0; i < drives; i++)
		{
			wctomb(strbuffer, lpBuffer[i * 4]);
			drive = strbuffer[0];
			drive.append(":/");
			this->TreeFromDirectory(Utility->toCharStar(&drive), 0);
		}
	}
	else
	{
		TreeFromDirectory(Utility->toCharStar(FilePath), 0);
	}
	Console->Log.flush();

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Command took: " + to_string(duration)));
}

string* CommandTree::getName()
{
	return new string("Tree");
}

string* CommandTree::getHelp()
{
	return new string("Shows a tree structure just like CMDs. Lets you get an idea of the directory structure while using command line.");
}

void CommandTree::TreeFromDirectory(char* name, unsigned __int32 indent)
{
	register DIR *dir;
	register struct dirent *entry;
	register string* ToLog = new string();

	if (!(dir = opendir(name)))
	{
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (ControlCPressed)
		{
			break;
		}
		if (entry->d_type == DT_DIR)
		{
			char path[MAX_PATH];
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
				continue;
			}
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			*ToLog = "";
			register __int64 i = 0;

			while (i != indent)
			{
				ToLog->append(" ");
				++i;
			}
			ToLog->append(path);
			Console->putLine(ToLog->c_str());
			TreeFromDirectory(path, indent + 2);
		}
		else
		{
			*ToLog = "";
			register __int64 i = 0;

			while (i != indent)
			{
				ToLog->append(" ");
				++i;
			}
			ToLog->append(entry->d_name);
			Console->putLine(ToLog->c_str());
		}
	}
	closedir(dir);
	delete ToLog;
}