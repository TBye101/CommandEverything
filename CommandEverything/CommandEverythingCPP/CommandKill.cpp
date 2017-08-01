#include "stdafx.h"
#include "CommandKill.h"


CommandKill::CommandKill()
{
}


CommandKill::~CommandKill()
{
}

bool CommandKill::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "kill");
}

void CommandKill::Run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() < 2)
	{
		Console->WriteLine("Missing an argument!");
	}
	else
	{

		transform(Parsed->Words->at(1).begin(), Parsed->Words->at(1).end(), Parsed->Words->at(1).begin(), ::tolower);
		if (Parsed->Words->at(1) == "list")
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
				MSG = *Utility->GetFileName(aProcesses[i]);

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
			int pID = std::atoi(Parsed->Words->at(1).c_str());
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
				Console->WriteLine(&to_string(GetLastError()));
			}
		}
	}
}

string* CommandKill::GetName()
{
	return new string("Kill");
}

string* CommandKill::GetHelp()
{
	return new string("\"Kill (process name here)\" kills the specified process. \"Kill list\ lists all processes running.");
}
