#pragma once
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
	ofstream log;

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
};