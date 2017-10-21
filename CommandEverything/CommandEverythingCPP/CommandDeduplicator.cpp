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
	//compareFileName a;
	//string x = "a:/a.txt";
	//string y = "b:/b.txt";
	//string z = "a:/a.txta";
	//cout << a(x, x);
	//cout << a(x, y);
	//cout << !a(y, x);
	//cout << a(y, x);
	//cout << a(x, z);
	//cout << a(z, x);
	clock_t start;
	double duration;
	start = clock();

	this->fileNameLog = Utility->initializeNewLogAndReader("deduplicator_file_names");
	this->hashLog = Utility->initializeNewLogAndReader("deduplicator_file_hashes");
	this->duplicatesLog = Utility->initializeNewLog("deduplicator_duplicates_found");
	
	//this->fileNameLog << "Test";
	//this->fileNameLog.flush();

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
			string namePath = string(name + string(entry->d_name));
			string sha;
			if (this->readContentsOfFile(namePath, sha))
			{
				//this->strHasher(sha);
				sha256(sha);
				this->fileData.insert(pair<string, string>(sha, namePath));
			}
		}
	}
	closedir(dir);
}

bool CommandDeduplicator::readContentsOfFile(string& path, string& out)
{
	ifstream fl;
	if (path[4] != '$')
	{
		int i = 0;
	}
	path.replace(2, 3, "\\");
	Utility->myreplace(path, "/", "\\");

	fl.open(path, ofstream::in);

	if (!fl)
	{
  		Console->WriteLine(&("Failed to open file: " + string(path)));
		Utility->DescribeIosFailure(fl);
		return false;
	}
	else
	{
		string line;
		string ret;
		unsigned __int64 i = 0;
		while (getline(fl, line))
		{
			ret.append(line);
		}

		fl.close();
		out = ret;
		return (ret.empty());
	}
}