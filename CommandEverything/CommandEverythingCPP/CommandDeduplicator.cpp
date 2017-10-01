#include "stdafx.h"
#include "CommandDeduplicator.h"

CommandDeduplicator::CommandDeduplicator()
{
}


CommandDeduplicator::~CommandDeduplicator()
{
}

bool CommandDeduplicator::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->words->at(0) == "deduplicate");
}

void CommandDeduplicator::run(ParsedCommand* Parsed)
{
	TPool->enqueue(&CommandDeduplicator::seperateThread, this);
}

string* CommandDeduplicator::getName()
{
	return new string("Deduplicate");
}

string* CommandDeduplicator::getHelp()
{
	return new string("Writes to a file all duplicates found in the file system.");
}

void CommandDeduplicator::seperateThread()
{
	clock_t start;
	double duration;
	start = clock();

	this->fileNameLog = Utility->initializeNewLogAndReader("deduplicator file names", &this->fileNameLogReader);
	this->hashLog = Utility->initializeNewLogAndReader("deduplicator file hashes", &this->hashLogReader);
	this->duplicatesLog = Utility->initializeNewLog("deduplicator duplicates found");

	register unsigned long mydrives = 100; // buffer length
	register wchar_t lpBuffer[100]; // buffer for drive string storage
	register unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer);
	register unsigned __int32 drives = test / 4;
	string drive = "";
	char strbuffer[64];
	unsigned __int64 files;

	//Generate total number of files.
	for (register unsigned __int8 i = 0; i < drives; i++)
	{
		wctomb(strbuffer, lpBuffer[i * 4]);
		drive = strbuffer[0];
		drive.append(":/");
		files = Utility->graphicCalculateFilesIn(drive.c_str());
		Console->WriteLine(&("Number of files found on " + drive + ": " + to_string(files)));
		this->totalFiles += files;
	}

	Utility->logLine(&duplicatesLog, (to_string(this->totalFiles) + " files found to deduplicate").c_str());

	//Iterates over all files.
	for (register unsigned __int8 i = 0; i < drives; i++)
	{
		wctomb(strbuffer, lpBuffer[i * 4]);
		drive = strbuffer[0];
		drive.append(":/");

		//Indexes all files on drive.
		this->fileIterator(Utility->toCharStar(&drive));
	}

	this->fileNameLog.close();

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Deduplicator took: " + to_string(duration)));
}

void CommandDeduplicator::fileIterator(char* name)
{
	DIR *dir;
	struct dirent *entry;

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
			this->fileIterator(path);
		}
		else
		{
			//File found. Work on it.
			this->addNameToIndex(entry->d_name, 0, -1);
		}
	}
	closedir(dir);
}

unsigned __int64 CommandDeduplicator::addNameToIndex(char* path, unsigned __int64 start, unsigned __int64 end)
{
	//If we need to fetch the size of our index...
	if (end == -1)
	{

	}
}
