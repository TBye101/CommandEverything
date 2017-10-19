#pragma once

#include "SHA256.h"

#include <iostream>
#include <fstream>
#include <map>

#define NO_DUPLICATE 0
#define SAME_NAME 1
#define SAME_CONTENTS 2
#define SAME_NAME_AND_CONTENTS 3

#define EQUAL 0
#define GREATER_THAN 1
#define LESS_THAN 2


/// <summary>
/// A comparator that compares strings with a file path, but only compares the filename.
/// </summary>
class compareFileName
{
public:
	bool operator()(string path1, string path2)
	{
		unsigned __int64 oneSize = path1.size();
		unsigned __int64 twoSize = path2.size();
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
			if (path1[i] > path2[i])
			{
				return false;
			}
			if (path1[i] < path2[i])
			{
				return true;;
			}
			++i;
		}

		return true;
	}
};

class CommandDeduplicator : public ICommand
{
public:
	CommandDeduplicator();
	~CommandDeduplicator();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
private:
	/// <summary>
	/// Holds all known data about the files found.
	/// Key: filename
	/// Value: hash
	/// </summary>
	map<string, string, compareFileName>* fileData = new map<string, string, compareFileName>();
	compareFileName* Comparator = new compareFileName();

	/// <summary>
	/// The file where we write down each file name.
	/// </summary>
	fstream fileNameLog;

	/// <summary>
	/// The file where we keep our hashes, that correspond with each file.
	/// </summary>
	fstream hashLog;

	/// <summary>
	/// The file where we talk about which files have duplicates, and how identical they are.
	/// </summary>
	ofstream duplicatesLog;

	/// <summary>
	/// Our hashing algorithm is in this class.
	/// </summary>
	SHA256 sha;

	/// <summary>
	/// Used to shift text lines down.
	/// </summary>
	string toShift1;

	/// <summary>
	/// The total number of files found.
	/// </summary>
	unsigned __int64 totalFiles = 0;

	/// <summary>
	/// The number of files we have checked.
	/// </summary>
	unsigned __int64 filesDone = 0;

	/// <summary>
	/// Number of files found with duplicate names.
	/// </summary>
	unsigned __int64 duplicateNameFiles = 0;

	/// <summary>
	/// The number of files found that were identical.
	/// </summary>
	unsigned __int64 identicalFiles = 0;

	/// <summary>
	/// The entry point for the separate thread to check for duplicates.
	/// </summary>
	void seperateThread();

	/// <summary>
	/// Iterates over all accessible files in the directory
	/// </summary>
	void fileIterator(char* dir);

	/// <summary>
	/// Reads the contents of the specified file, and returns them as a string.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	string readContentsOfFile(char* path);

	/// <summary>
	/// Removes the path from the string, leaving only the name and file extension.
	/// </summary>
	/// <param name="path"></param>
	void removePath(char* path);
	string removePath(string* path);
};