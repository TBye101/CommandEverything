#include "stdafx.h"
#include "CommandKill.h"

CommandKill::CommandKill()
{
}

CommandKill::~CommandKill()
{
}

bool CommandKill::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->words->at(0) == "kill");
}

void CommandKill::run(ParsedCommand* Parsed)
{
	if (Parsed->words->size() < 2)
	{
		Console->WriteLine("Missing an argument!");
	}
	else
	{
		transform(Parsed->words->at(1).begin(), Parsed->words->at(1).end(), Parsed->words->at(1).begin(), ::tolower);
		if (Parsed->words->at(1) == "list")
		{
			//List processes
			// Get the list of process identifiers.  
			register unsigned long aProcesses[1024];
			unsigned long cbNeeded;
			register unsigned long numberOfProcesses;

			EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
			numberOfProcesses = cbNeeded / sizeof(unsigned long);

			string MSG;
			string Cant = "Unknown processes: ";
			for (register unsigned int i = 0; i < numberOfProcesses; i++)
			{
				MSG = *Utility->getFileName(aProcesses[i]);

				if (MSG == "")
				{
					Cant.append(to_string(aProcesses[i]));
					Cant.append(", ");
				}
				else
				{
					MSG.append(": ");
					MSG.append(to_string(aProcesses[i]));
					Console->WriteLine(&MSG);
				}
			}
			Console->WriteLine(&Cant);
		}
		else
		{
			//Kill process
			int pID = std::atoi(Parsed->words->at(1).c_str());
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pID);
			if (TerminateProcess(hProcess, 1))
			{
				string msg = "Successfully killed a process.";
				Console->WriteLine(&msg);
			}
			else
			{
				string msg = "Failed to kill a process";
				Console->WriteLine(&msg);
			}
		}
	}
}

string* CommandKill::getName()
{
	return new string("Kill");
}

string* CommandKill::getHelp()
{
	return new string("\"Kill (process ID here)\" kills the specified process. \"Kill list\" lists all processes running.");
}