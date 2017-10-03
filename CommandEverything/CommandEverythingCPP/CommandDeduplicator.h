#pragma once

#include "SHA256.h"

#include <iostream>
#include <fstream>

#define NO_DUPLICATE 0
#define SAME_NAME 1
#define SAME_CONTENTS 2
#define SAME_NAME_AND_CONTENTS 3

#define EQUAL 0
#define GREATER_THAN 1
#define LESS_THAN 2

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
	fstream fileNameLog;

	/// <summary>
	/// The file where we keep our hashes, that corrospond with each file.
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
	/// 
	/// position is the index in the hash log, and the file name log.
	/// </summary>
	/// <returns></returns>
	unsigned __int8 hasDuplicate(unsigned __int64 position);

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
	/// Compares the strings, and returns:
	/// EQUAL, GREATER_THAN, or LESS_THAN depending on how the strings compare.
	/// </summary>
	/// <param name="one"></param>
	/// <param name="two"></param>
	/// <returns></returns>
	unsigned __int8 compareStrings(char* one, char* two);

	/// <summary>
	/// Returns the size of our file name index.
	/// </summary>
	/// <returns></returns>
	unsigned __int64 getFileNameIndexSize();

	/// <summary>
	/// Returns the name of the file at the specified position in the index.
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	char* getNameFromIndex(unsigned __int64 position);

	/// <summary>
	/// Returns the hash from the specified position in the index.
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	char* getHashFromIndex(unsigned __int64 position);

	/// <summary>
	/// Inserts the string at the specified position in the name index.
	/// </summary>
	/// <param name="position"></param>
	void insertInNameIndex(unsigned __int64 position, char* str);

	/// <summary>
	/// Reads the contents of the specified file, and returns them as a string.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	string readContentsOfFile(char* path);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool isDuplicateInName(char* str);

	/// <summary>
	/// Removes the path from the string, leaving only the name and file extension.
	/// </summary>
	/// <param name="path"></param>
	void removePath(char* path);
};