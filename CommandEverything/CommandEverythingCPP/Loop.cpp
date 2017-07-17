#include "stdafx.h"
#include "Loop.h"

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

			//if there is valid input....
			if (parsed->Words->size() > 0)
			{
				//convert the first word into the lower case
				string first = parsed->Words->at(0);
				transform(first.begin(), first.end(), first.begin(), ::tolower);
				parsed->Words->at(0) = first;

				register size_t length = Commands->size();

				for (register size_t i = 0; i < length; ++i)
				{
					if (Commands->at(i)->ShouldRunThisCommand(parsed))
					{
						//string name = "-------------------- ";
						//name.append(*Commands->at(i)->GetName());
						//name.append(" --------------------");

						//Console->WriteLine(&name);
						//cout << "\r\n";
						Commands->at(i)->Run(parsed);
						//cout << "\r\n";
						//Console->WriteLine(&name);
						break;
					}
				}
			}

			FreeUpMemory();
			delete parsed;
		}
		catch (exception* e)
		{
			string *message = new string("!Error!: ");
			message->append(e->what());
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

	register size_t length = Commands->size();
	for (register size_t i = 0; i < length; i++)
	{
		CommandNames->push_back(Commands->at(i)->GetName());
	}
	std::sort(CommandNames->begin(), CommandNames->end());

	this->FreeUpMemory();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void Loop::AddAllCommands()
{
	Commands->push_back(new CommandCd());
	Commands->push_back(new CommandDefend());
	Commands->push_back(new CommandExit());
	Commands->push_back(new CommandHelp());
	Commands->push_back(new CommandList());
	Commands->push_back(new CommandStart());
}