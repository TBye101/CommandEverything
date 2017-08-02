#include "stdafx.h"
#include "CommandTree.h"

CommandTree::CommandTree()
{
}


CommandTree::~CommandTree()
{
}

bool CommandTree::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "tree");
}

void CommandTree::Run(ParsedCommand* Parsed)
{	
	//If no working directory, do all drives!
	if (*FilePath == "")
	{
		unsigned long mydrives = 100; // buffer length
		wchar_t lpBuffer[100]; // buffer for drive string storage
		unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer);
		__int8 drives = test / 4;
		string drive = "";
		char strbuffer[64];


		for (unsigned __int8 i = 0; i < drives; i++)
		{
			wctomb(strbuffer, lpBuffer[i * 4]);
			drive = strbuffer[0];
			drive.append(":/");
			this->TreeFromDirectory(Utility->ToCharStar(&drive), 0);
		}
	}
	else
	{
		this->TreeFromDirectory(Utility->ToCharStar(FilePath), 0);
	}
}

string* CommandTree::GetName()
{
	return new string("Tree");
}

string* CommandTree::GetHelp()
{
	return new string("Shows a tree structure just like CMDs. Lets you get an idea of the directory structure while using command line.");
}

void CommandTree::TreeFromDirectory(char* name, __int64 indent)
{
	DIR *dir;
	struct dirent *entry;
	string* ToLog = new string();

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
			printf("%*s[%s]\n", indent, "", entry->d_name);
			*ToLog = "";
			register __int64 i = 0;

			while (i != indent)
			{
				ToLog->append(" ");
				++i;
			}
			ToLog->append(path);
			Console->Log << *ToLog + "\r\n";
			this->TreeFromDirectory(path, indent + 2);
		}
		else
		{
			printf("%*s- %s\n", indent, "", entry->d_name);
			
			*ToLog = "";
			register __int64 i = 0;

			while (i != indent)
			{
				ToLog->append(" ");
				++i;
			}
			ToLog->append(entry->d_name);
			Console->Log << *ToLog + "\r\n";

		}
	}
	closedir(dir);
	delete ToLog;
}