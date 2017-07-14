#include "stdafx.h"
#include "CommandDefend.h"


CommandDefend::CommandDefend()
{
}


CommandDefend::~CommandDefend()
{
}

bool CommandDefend::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "defend");
}

void CommandDefend::Run(ParsedCommand* Parsed)
{
	//There wasn't another word to determine whether we were to start or stop
	if (Parsed->Words->size() < 2)
	{
		string error = "Missing required command argument.";
		Console->WriteLine(&error);
		return;
	}

	if (Parsed->Words->at(1) == "start")
	{
		this->DefendStart();
		return;
	}

	if (Parsed->Words->at(1) == "stop")
	{
		this->DefendStop();
		return;
	}

	string error = "Invalid command argument";
	Console->WriteLine(&error);
}

string* CommandDefend::GetName()
{
	return new string("Defend");
}

string* CommandDefend::GetHelp()
{
	return new string("Kills all applications that were not running before Defend was started.\r\n Usage: Defend Start to start Defend, Defend Stop to stop Defend.");
}

bool CommandDefend::DoesProcessExistInList(DWORD pID, DWORD numberOfProcesses)
{
	register DWORD length = numberOfProcesses;

	for (register size_t i = 0; i < numberOfProcesses; i++)
	{
		if (pID == this->AllowedProcesses[i])
		{
			return true;
		}
	}

	return false;
}

void CommandDefend::DefendStart()
{
	if (!this->DefenseThread)
	{
		delete this->DefenseThread;
	}

	this->DefenseThread = new thread(&CommandDefend::Defend, this);
}

void CommandDefend::DefendStop()
{
	delete this->DefenseThread;
}

void CommandDefend::Defend()
{
	// Get the list of process identifiers.  
	register DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD numberOfProcesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	numberOfProcesses = cbNeeded / sizeof(DWORD);
	//Copies the contents of aProcesses to this->AllowedProcesses.
	memcpy(this->AllowedProcesses, aProcesses, sizeof(this->AllowedProcesses));

	register size_t i = 0;
	HANDLE hProcess;
	while (true)
	{
		EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
		i = 0;
		numberOfProcesses = cbNeeded / sizeof(DWORD);

		for (i = 0; i < numberOfProcesses; i++)
		{
			if (!this->DoesProcessExistInList(aProcesses[i], numberOfProcesses))
			{
				//Get process handle.
				hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);

				//Kill process.
				if (TerminateProcess(hProcess, 1))
				{
					Console->WriteLine("Killed a process.");
				}
				else
				{
					Console->WriteLine(&to_string(GetLastError()));
					Console->WriteLine("Failed to kill a process.");
				}
			}
		}
	}
}