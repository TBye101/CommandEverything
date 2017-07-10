// CommandEverythingCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ICommand.h"
#include "cmdparser.hpp"

#include <vector>

using namespace std;

vector<ICommand*>* Commands = new vector<ICommand*>();

int main(int argc, char* argv[])
{
	Startup();
    return 0;
}

/// <summary>
/// The main logic loop of the console.
/// </summary>
void MainLoop()
{
	try
	{

	}
	catch (exception e)
	{

	}
}

/// <summary>
/// Supposedly frees up all of that memory we were forgetting about.
/// </summary>
void FreeUpMemory()
{
	ToDelete->swap(*ToDelete);
}

/// <summary>
/// Has some startup logic.
/// </summary>
void Startup()
{
	AddAllCommands();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void AddAllCommands()
{

}