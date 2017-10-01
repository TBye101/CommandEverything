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
	/// The entry point for the seperate thread to check for duplicates.
	/// </summary>
	void seperateThread();

	/// <summary>
	/// Iterates over all accessible files in the tree.
	/// </summary>
	void fileIterator();
};

