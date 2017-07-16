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
		Console->WriteLine(&error, 4);
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
	Console->WriteLine(&error, 3);
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
	DWORD CID = GetCurrentProcessId();
	while (true)
	{
		EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
		i = 0;
		numberOfProcesses = cbNeeded / sizeof(DWORD);

		for (i = 0; i < numberOfProcesses; i++)
		{
			if (!this->DoesProcessExistInList(aProcesses[i], numberOfProcesses)&& aProcesses[i] != CID)
			{
				//Get process handle.
				hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);

				wchar_t* buffer = new wchar_t();
				GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH);
				wstring ws(buffer);
				string converted(ws.begin(), ws.end());
				//Kill process.
				if (TerminateProcess(hProcess, 1))
				{
					string msg = "Killed a process: ";
					msg.append(converted);
					Console->WriteLine(&msg, 6);
				}
				else
				{
					string msg = "Failed to kill a process: ";
					msg.append(converted);
					Console->WriteLine(&msg, 4);
					Console->WriteLine(&to_string(GetLastError()), 3);
				}
				delete buffer;
			}
		}
	}
}