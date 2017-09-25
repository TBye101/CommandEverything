#include "stdafx.h"
#include "RAIDFile.h"

RAIDFile::RAIDFile(wstring* rootPathArray, unsigned __int8 arraySize, const char* path, const char* fileName)
{
	wstring* paths = (wstring*)malloc((sizeof (wstring) * arraySize));
	memcpy(paths, rootPathArray, (sizeof(wstring) * arraySize));

	string spath = path;
	string sfileName = fileName;
	register unsigned __int8 i = 0;

	while (i != arraySize)
	{
		paths[i].append(wstring(spath.begin(), spath.end()));
		paths[i].append(wstring(sfileName.begin(), sfileName.end()));
		++i;
	}

	this->logArray = new ofstream[arraySize];
	this->logArraySize = arraySize;

	i = 0;
	while (i != arraySize)
	{
		this->logArray[i].open(paths[i], fstream::out);
		if (!this->logArray[i])
		{
			cout << "Can't create log file! RAID error!\r\n";
			cout.flush();
		}

		++i;
	}
}

RAIDFile::~RAIDFile()
{
}

void RAIDFile::writeLine(const char* str)
{
	string* time = this->GetTime();
	this->logArray[this->logNext] << *time;
	this->logArray[this->logNext] << str;
	this->logArray[this->logNext] << "\r\n";
	this->logArray[this->logNext].flush();
	delete time;
	++this->logNext;
	
	if (this->logNext > this->logArraySize - 1)
	{
		this->logNext = 0;
	}
}

string* RAIDFile::GetTime()
{
	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]: ");
	return Formatted;
}
