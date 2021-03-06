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
	return (Parsed->words->at(0) == "tree");
}

void CommandTree::run(ParsedCommand* Parsed)
{
	TPool->enqueue(&CommandTree::runThreaded, this);
}

void CommandTree::runThreaded()
{
	clock_t start;
	double duration;
	start = clock();
	this->initializeLog();
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
			this->treeFromDirectory(Utility->toCharStar(&drive), 0);
		}
	}
	else
	{
		treeFromDirectory(Utility->toCharStar(FilePath), 0);
	}
	Console->Log.flush();
	this->treeLog.close();

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

void CommandTree::treeFromDirectory(char* name, unsigned __int32 indent)
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
			this->logLine(ToLog->c_str());
			treeFromDirectory(path, indent + 2);
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
			this->logLine(ToLog->c_str());
		}
	}
	closedir(dir);
	delete ToLog;
}

void CommandTree::logLine(const char * str)
{
	char* time = Utility->getTime();
	this->treeLog << time;
	this->treeLog << str;
	this->treeLog << "\r\n";
}

void CommandTree::initializeLog()
{
	string path;

	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;
	path = converter.to_bytes(*Files->currentInstanceLog);

	path.append("\\");
	path.append(Utility->getTime());
	path.append(" tree");
	path.append(".txt");

	this->treeLog.open(path, fstream::out);

	if (!this->treeLog)
	{
		cout << "Can't access file!\r\n";
	}
	else
	{
		//cout << "Log has initialized successfully\r\n";
	}
}
