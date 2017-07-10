#include "stdafx.h"
#include "Loop.h"
#include "ParsedCommand.h"
#include <Windows.h>
#include "Filing.h"


Loop::Loop()
{
}


Loop::~Loop()
{
	ToDelete->push_back(this->parsed);
}


/// <summary>
/// The main logic loop of the console.
/// </summary>
void Loop::MainLoop()
{
	while (true)
	{
		try
		{
			getline(cin, Input);
			Console->LogLine(&Input);

			parsed = new ParsedCommand(&Input);

			register size_t length = Commands->size();

			for (register size_t i = 0; i < length; ++i)
			{
				if (Commands->at(i)->ShouldRunThisCommand(parsed))
				{
					Commands->at(i)->Run(parsed);
					FreeUpMemory();
					break;
				}
			}
		}
		catch (exception e)
		{
			string *message = new string(e.what());
			Console->WriteLine(message);
			ToDelete->push_back(message);
			continue;
		}
	}
}

/// <summary>
/// Supposedly frees up all of that memory we were forgetting about.
/// </summary>
void Loop::FreeUpMemory()
{
	ToDelete->swap(*ToDelete);
}

/// <summary>
/// Has some startup logic.
/// </summary>
void Loop::Startup()
{
	Filing* fil = new Filing();
	fil->Startup();
	ToDelete->push_back(fil);

	AddAllCommands();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void Loop::AddAllCommands()
{

}