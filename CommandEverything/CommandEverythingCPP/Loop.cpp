#include "stdafx.h"
#include "Loop.h"

Loop::Loop()
{
}

Loop::~Loop()
{
	delete this->parsed;
}

/// <summary>
/// The main logic loop of the console.
/// </summary>
void Loop::MainLoop()
{
	bool CommandRun = false;

	while (true)
	{
		try
		{
			CommandRun = false;
			getline(cin, Input);
			Console->LogLine(Input.c_str());

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
					if (Commands->at(i)->shouldRunThisCommand(parsed))
					{
						Commands->at(i)->run(parsed);
						CommandRun = true;
						break;
					}
				}

				if (!CommandRun)
				{
					Console->WriteLine("Invalid Command!");
				}
			}

			delete parsed;
		}
		catch (exception* e)
		{
			string *message = new string("!Error!: ");
			message->append(e->what());
			Console->WriteLine(message);
			delete message;
			continue;
		}
	}
}

/// <summary>
/// Has some startup logic.
/// </summary>
void Loop::Startup()
{
	Files->Startup();
	AddAllCommands();

	register unsigned __int64 length = Commands->size();
	for (register unsigned __int64 i = 0; i < length; i++)
	{
		CommandNames->push_back(Commands->at(i)->getName());
	}
	std::sort(CommandNames->begin(), CommandNames->end());

	this->ProtectProcess();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void Loop::AddAllCommands()
{
	Commands->push_back(new CommandCd());
	Commands->push_back(new CommandCrash());
	Commands->push_back(new CommandDefend());
	//Commands->push_back(new CommandDecrypt());
	Commands->push_back(new CommandDelete());
	//Commands->push_back(new CommandEncrypt());
	Commands->push_back(new CommandExecute());
	Commands->push_back(new CommandExit());
	Commands->push_back(new CommandHack());
	Commands->push_back(new CommandHelp());
	Commands->push_back(new CommandHide());
	Commands->push_back(new CommandKill());
	Commands->push_back(new CommandList());
	Commands->push_back(new RunScript());
	Commands->push_back(new CommandSpecs());
	Commands->push_back(new CommandStart());
	Commands->push_back(new CommandTree());
}

void Loop::ProtectProcess()
{
	HANDLE hProcess = GetCurrentProcess();
	PACL pEmptyDacl;
	unsigned long dwErr;

	// using malloc guarantees proper alignment
	pEmptyDacl = (PACL)malloc(sizeof(ACL));

	if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION))
	{
		dwErr = GetLastError();
	}
	else
	{
		dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);
	}

	free(pEmptyDacl);
}
