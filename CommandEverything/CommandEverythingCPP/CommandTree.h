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

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
private:
	/// <summary>
	/// Does a directory tree from the specified path.
	/// </summary>
	/// <param name="Path"></param>
	/// <param name=""></param>
	static void TreeFromDirectory(char* Path, unsigned __int32 indent);
};