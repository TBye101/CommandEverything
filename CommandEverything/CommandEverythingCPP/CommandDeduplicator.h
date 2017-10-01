#pragma once

#include <iostream>
#include <fstream>

#define NO_DUPLICATE 0
#define SAME_NAME 1
#define SAME_CONTENTS 2
#define SAME_NAME_AND_CONTENTS 3

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
	/// The file where we write down each file name.
	/// </summary>
	ofstream fileNameLog;
	ifstream fileNameLogReader;

	/// <summary>
	/// The file where we keep our hashes, that corrospond with each file.
	/// </summary>
	ofstream hashLog;
	ifstream hashLogReader;

	/// <summary>
	/// The file where we talk about which files have duplicates, and how identical they are.
	/// </summary>
	ofstream duplicatesLog;

	/// <summary>
	/// The total number of files found.
	/// </summary>
	unsigned __int64 totalFiles;

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
	/// The entry point for the seperate thread to check for duplicates.
	/// </summary>
	void seperateThread();

	/// <summary>
	/// Iterates over all accessible files in the directory
	/// </summary>
	void fileIterator(char* dir);

	/// <summary>
	/// Returns NO_DUPLICATE if the file system has no file named the same as it.
	/// Returns SAME_NAME if the file system has a file with the same name as it.
	/// Returns SAME_CONTENTS if the file system has a file with the same contents as it.
	/// Returns SAME_NAME_AND_CONTENTS if the file system has a file with the same name and contents as it.
	/// Writes this information to the appropiete log.
	/// </summary>
	/// <param name="path">The path to the file in question.</param>
	/// <returns></returns>
	unsigned __int8 hasDuplicate(char* path);

	/// <summary>
	/// Inserts the file name into the correct position in the index file.
	/// Returns the position, so the hash might be put into the corrosponding place.
	/// </summary>
	/// <param name="path"></param>
	/// <param name="end">The maximum size of our range of options.
	/// If -1, then we will allow the maximum amount of options possible.</param>
	/// <returns></returns>
	unsigned __int64 addNameToIndex(char* path, unsigned __int64 start, unsigned __int64 end);
	void addHashToIndex(char* hash, unsigned __int64 position);

	/// <summary>
	/// Returns the size of our file name index.
	/// </summary>
	/// <returns></returns>
	unsigned __int64 getIndexSize();
};