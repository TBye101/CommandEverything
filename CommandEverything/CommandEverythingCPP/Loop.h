#pragma once
#include "ICommand.h"

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

	/// <summary>
	/// All known commands.
	/// </summary>
	vector<ICommand*>* Commands = new vector<ICommand*>();

private:
	/// <summary>
	/// Adds all of the known commands at startup.
	/// </summary>
	void AddAllCommands();
};

