#include "stdafx.h"
#include "Loop.h"
#include "ParsedCommand.h"


Loop::Loop()
{
}


Loop::~Loop()
{
}


/// <summary>
/// The main logic loop of the console.
/// </summary>
void Loop::MainLoop()
{
	string Input;
	ParsedCommand* parsed;

	while (true)
	{
		try
		{
			getline(cin, Input);
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
			//TODO: write this to the console.
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
	AddAllCommands();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void Loop::AddAllCommands()
{
	int a = 0;
}