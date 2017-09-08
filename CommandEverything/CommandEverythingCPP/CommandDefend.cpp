#include "stdafx.h"
#include "CommandDefend.h"

CommandDefend::CommandDefend()
{
}

CommandDefend::~CommandDefend()
{
}

bool CommandDefend::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "defend");
}

void CommandDefend::run(ParsedCommand* Parsed)
{
	//There wasn't another word to determine whether we were to start or stop
	if (Parsed->Words->size() < 2)
	{
		string error = "Missing required command argument.";
		Console->WriteLine(&error);
		return;
	}
	string second = Parsed->Words->at(1);
	std::transform(second.begin(), second.end(), second.begin(), ::tolower);

	if (second == "start")
	{
		this->DefendStart();
		return;
	}

	if (second == "stop")
	{
		this->DefendStop();
		return;
	}

	string error = "Invalid command argument";
	Console->WriteLine(&error);
}

string* CommandDefend::getName()
{
	return new string("Defend");
}

string* CommandDefend::getHelp()
{
	return new string("Kills all applications that were not running before Defend was started.\r\n Usage: Defend Start to start Defend, Defend Stop to stop Defend.");
}

bool CommandDefend::DoesProcessExistInList(unsigned int pID, unsigned int numberOfProcesses)
{
	register unsigned __int64 length = numberOfProcesses;
	register unsigned __int64 i;

	for (i = 0; i < numberOfProcesses; i++)
	{
		if (pID == this->AllowedProcesses[i])
		{
			return true;
		}
	}
	length = this->FailedHitList.size();
	for (i = 0; i < length; i++)
	{
		if (pID == this->FailedHitList[i])
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
	this->StopThread = true;
}

void CommandDefend::Defend()
{
	this->StopThread = false;
	// Get the list of process identifiers.  
	register unsigned long aProcesses[1024];
	unsigned long cbNeeded;
	unsigned long numberOfProcesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	numberOfProcesses = cbNeeded / sizeof(unsigned long);
	//Copies the contents of aProcesses to this->AllowedProcesses.
	memcpy(this->AllowedProcesses, aProcesses, sizeof(this->AllowedProcesses));

	register unsigned int i = 0;
	void* hProcess;
	unsigned long CID = GetCurrentProcessId();
	while (true)
	{
		if (this->StopThread)
		{
			break;
		}
		EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
		i = 0;
		numberOfProcesses = cbNeeded / sizeof(unsigned long);

		for (i = 0; i < numberOfProcesses; i++)
		{
			if (!this->DoesProcessExistInList(aProcesses[i], numberOfProcesses) && aProcesses[i] != CID)
			{
				//Get process handle.
				hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);
				void* info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

				wchar_t buffer[MAX_PATH];
				GetModuleFileNameEx(info, NULL, buffer, MAX_PATH);
				wstring ws(buffer);
				string converted(ws.begin(), ws.end());
				//https://stackoverflow.com/questions/45288937/avoid-killing-a-specific-process-c?noredirect=1#comment77540465_45288937

				char filename[MAX_PATH];
				char extension[MAX_PATH];
				_splitpath_s(converted.c_str(), NULL, 0, NULL, 0, filename, MAX_PATH, extension, MAX_PATH);

				if (filename[0] != 'c' || filename[1] != 'o' || filename[2] != 'n' || filename[3] != 'h'
					|| filename[4] != 'o' || filename[5] != 's' || filename[6] != 't'
					|| extension[0] != '.' || extension[1] != 'e' || extension[2] != 'x'
					|| extension[3] != 'e')
				{
					//Kill process.
					if (TerminateProcess(hProcess, 1))
					{
						string msg = "Killed a process: ";
						msg.append(converted);
						Console->WriteLine(&msg);
					}
					else
					{
						this->FailedHitList.push_back(aProcesses[i]);
						string msg = "Failed to kill a process: ";
						msg.append(converted);
						Console->WriteLine(&msg);
						Console->WriteLine(&to_string(GetLastError()));
					}
				}
			}
		}
	}
}