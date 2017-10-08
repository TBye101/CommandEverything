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

	this->fileNameLog = Utility->initializeNewLogAndReader("deduplicator file names");
	this->hashLog = Utility->initializeNewLogAndReader("deduplicator file hashes");
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

			//Insert it into the file name index
			unsigned __int64 position = this->addNameToIndex(entry->d_name, 0, -1);

			//Get the hash of the file.
			string sha = sha256(this->readContentsOfFile(entry->d_name));

			//Add the hash to the proper index.
			this->addHashToIndex(Utility->toCharStar(&sha), position);
		}
	}
	closedir(dir);
}

unsigned __int8 CommandDeduplicator::hasDuplicate(unsigned __int64 position)
{
	char* nameMessage;
	char* contentsMessage;

	bool nameDup = this->isDuplicateInName(position, nameMessage);
	bool contentsDup = this->isDuplicateInContents(position, contentsMessage);

	if (nameMessage != NULL)
	{
		Utility->logLine(&this->duplicatesLog, nameMessage);
	}

	if (contentsMessage != NULL)
	{
		Utility->logLine(&this->duplicatesLog, contentsMessage);
	}

	delete nameMessage;
	delete contentsMessage;
	if (nameDup && contentsDup)
	{
		return SAME_NAME_AND_CONTENTS;
	}
	if (nameDup)
	{
		return SAME_NAME;
	}
	if (contentsDup)
	{
		return SAME_CONTENTS;
	}

	return NO_DUPLICATE;
}

unsigned __int64 CommandDeduplicator::addNameToIndex(char* path, unsigned __int64 start, unsigned __int64 end)
{
	char* withoutPath;

	strcpy(withoutPath, path);
	this->removePath(path);
	
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

		char* middleName = this->getNameFromIndex(middle);
		this->removePath(middleName);
		unsigned __int8 compareValue = this->compareStrings(path, middleName);

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
		delete middleName;
	}

	delete withoutPath;
}

void CommandDeduplicator::addHashToIndex(char* hash, unsigned __int64 position)
{
	string line;

	this->hashLog.seekg(0);

	unsigned __int64 i = 0;
	//skip to the position.
	while (i != position)
	{
		getline(this->hashLog, line);
		++i;
	}

	//Get the string we are about to overwrite.
	getline(this->hashLog, toShift1);
	//Jump up a position
	this->hashLog.seekg(i);
	//Write our str.
	this->hashLog << hash;

	//Shift everything down.
	while (getline(this->hashLog, line))
	{
		this->hashLog.seekg(i - 1);
		this->hashLog << line;
		++i;
	}

	//That last extra line.
	this->hashLog << line;
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

	//Reset the position of the stream.
	this->fileNameLog.seekg(0);

	unsigned __int64 i = 0;
	while (getline(this->fileNameLog, line))
	{
		++i;
	}

	return i;
}

char* CommandDeduplicator::getNameFromIndex(unsigned __int64 position)
{
	//Tell the disk to move to the position of the name.
	this->fileNameLog.seekg(position);

	string line;
	getline(this->fileNameLog, line);
	return Utility->toCharStar(&line);
}

char* CommandDeduplicator::getHashFromIndex(unsigned __int64 position)
{
	string line;

	//Tell the stream to move to the position of our hash.
	this->hashLog.seekg(position);
	getline(this->hashLog, line);
	return Utility->toCharStar(&line);
}

void CommandDeduplicator::insertInNameIndex(unsigned __int64 position, char* str)
{
	string line;

	this->fileNameLog.seekg(0);

	unsigned __int64 i = 0;
	//skip to the position.
	while (i != position)
	{
		getline(this->fileNameLog, line);
		++i;
	}

	//Get the string we are about to overwrite.
	getline(this->fileNameLog, toShift1);
	//Jump up a position
	this->fileNameLog.seekg(i);
	//Write our str.
	this->fileNameLog << str;

	//Shift everything down.
	while (getline(this->fileNameLog, line))
	{
		this->fileNameLog.seekg(i - 1);
		this->fileNameLog << line;
		++i;
	}
	
	//That last extra line.
	this->fileNameLog << line;
}

string CommandDeduplicator::readContentsOfFile(char* path)
{
	ifstream fl;
	fl.open(path, ofstream::in);

	string line = "";
	string ret = "";
	unsigned __int64 i = 0;
	while (getline(fl, line))
	{
		ret.append(line);
	}

	return ret;
}

bool CommandDeduplicator::isDuplicateInName(unsigned __int64 position, char* names)
{
	string* duplicates = new string();
	string line;
	char* name = this->getNameFromIndex(position);

	char* nameWithoutPath;
	strcpy(nameWithoutPath, name);
	this->removePath(nameWithoutPath);

	this->fileNameLog.seekg(0);

	while (getline(this->fileNameLog, line))
	{
		char* pathWithoutName = Utility->toCharStar(&line);
		this->removePath(pathWithoutName);
		if (this->compareStrings(name, pathWithoutName) == EQUAL)
		{
			if (duplicates->empty())
			{
				duplicates->append("Files with duplicate names: ");
				duplicates->append(name);
			}
			duplicates->append(line);
		}

		delete pathWithoutName;
	}

	delete nameWithoutPath;
	delete name;
	return false;
}

bool CommandDeduplicator::isDuplicateInContents(unsigned __int64 position, char* names)
{
	string* duplicates = new string();
	string line;
	char* hash = this->getHashFromIndex(position);

	this->hashLog.seekg(0);

	while (getline(this->hashLog, line))
	{
		char* nextHash = Utility->toCharStar(&line);
		if (this->compareStrings(hash, nextHash) == EQUAL)
		{
			if (duplicates->empty())
			{
				duplicates->append("Files with duplicate contents: ");
				duplicates->append(hash);
			}
			duplicates->append(line);
		}

		delete nextHash;
	}

	delete hash;
	return false;
}

