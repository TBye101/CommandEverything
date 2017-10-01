#pragma once

#include "ICommand.h"
#include "Header Only\dirent.h"

/// <summary>
/// Shows a tree structure of where the current working directory is.
/// </summary>
class CommandTree : public ICommand
{
public:
	CommandTree();
	~CommandTree();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
private:
	/// <summary>
	/// Does a directory tree from the specified path.
	/// </summary>
	/// <param name="Path"></param>
	/// <param name=""></param>
	void treeFromDirectory(char* Path, unsigned __int32 indent);

	/// <summary>
	/// Writes a line to the dedicated tree log file.
	/// </summary>
	/// <param name="str"></param>
	void logLine(const char* str);

	/// <summary>
	/// Where the thread pool is pointed to when running this command.
	/// </summary>
	void runThreaded();

	ofstream treeLog;
};