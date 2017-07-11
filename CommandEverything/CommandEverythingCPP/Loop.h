#pragma once
#include "ICommand.h"
#include "CommandHelp.h"

using namespace std;

/// <summary>
/// The main logic of the program.
/// </summary>
class Loop
{
public:
	Loop();
	~Loop();

	/// <summary>
	/// Frees up some memory, that we were waiting to delete.
	/// </summary>
	void FreeUpMemory();

	/// <summary>
	/// The main loop of the console.
	/// </summary>
	void MainLoop();

	/// <summary>
	/// Some startup logic.
	/// </summary>
	void Startup();

private:
	/// <summary>
	/// Adds all of the known commands at startup.
	/// </summary>
	void AddAllCommands();

	string Input;
	ParsedCommand* parsed;
};

