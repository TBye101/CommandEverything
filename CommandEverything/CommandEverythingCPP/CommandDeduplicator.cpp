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

	Utility->logLine(&duplicatesLog, (to_string(this->totalFiles) + " files found to check for duplicates").c_str());

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
		end = this->getFileNameIndexSize();
	}

	//If we don't have anything in the index yet...
	if (end == 0)
	{
		//Just add it.
		this->fileNameLog << path;
		this->fileNameLog << "\r\n";
	}
	else
	{
		//Start binary searching.
		unsigned __int64 middle = (start + end) / 2;
		unsigned __int8 compareValue = this->compareStrings(path, this->getNameFromIndex(middle));

		if (compareValue == GREATER_THAN)
		{
			//Insert position must be in the top half of the working range.
			this->addNameToIndex(path, middle, end);
		}
		else
		{
			if (compareValue == LESS_THAN)
			{
				//Insert position must be in the bottom half of the working range.
				this->addNameToIndex(path, 0, middle);
			}
			else
			{
				//We found it! middle is the insert position.
				this->insertInNameIndex(middle, path);
				return middle;
			}
		}
	}
}

unsigned __int8 CommandDeduplicator::compareStrings(char* one, char* two)
{
	//I hope these are null terminated
	unsigned __int64 oneSize = strlen(one);
	unsigned __int64 twoSize = strlen(two);
	register unsigned __int64 leastSize;
	register unsigned __int64 i = 0;

	//Get the size of the smaller string
	if (oneSize > twoSize || oneSize == twoSize)
	{
		leastSize = oneSize;
	}
	else
	{
		leastSize = twoSize;
	}

	while (i != leastSize)
	{
		if (one[i] > two[i])
		{
			return GREATER_THAN;
		}
		if (one[i] < two[i])
		{
			return LESS_THAN;
		}
		++i;
	}

	return EQUAL;
}

unsigned __int64 CommandDeduplicator::getFileNameIndexSize()
{
	string line;
	unsigned __int64 i = 0;
	while (getline(this->fileNameLogReader, line))
	{
		++i;
	}

	return i;
}

char* CommandDeduplicator::getNameFromIndex(unsigned __int64 position)
{
	string line;
	char* ret = Utility->toCharStar(&line);
	unsigned __int64 i = 0;
	while (i != position)
	{
		getline(this->fileNameLogReader, line);
		++i;
	}

	return ret;
}

char* CommandDeduplicator::getHashFromIndex(unsigned __int64 position)
{
	string line;
	char* ret = Utility->toCharStar(&line);
	unsigned __int64 i = 0;
	while (i != position)
	{
		getline(this->hashLogReader, line);
		++i;
	}

	return ret;
}