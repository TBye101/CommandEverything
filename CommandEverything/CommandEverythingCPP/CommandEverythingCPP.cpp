// CommandEverythingCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Loop.h"

using namespace std;

int WINAPI HandlerRoutine(_In_ unsigned long dwCtrlType)
{
	switch (dwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		ControlCPressed = true;
		return(true);
	default:
		return false;
	}
}

int main(int argc, char* argv[])
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	TPool = new ThreadPool(sysinfo.dwNumberOfProcessors);

	//Makes them think this thing runs faster than it actually will. :D
	Console->WriteLine(&(to_string((sysinfo.dwNumberOfProcessors / 2)) + " CPUs detected"));

	if (!SetConsoleCtrlHandler(HandlerRoutine, true))
	{
		Console->WriteLine("!Error!: Could not set event handler.");
	}

	Loop* Program = new Loop();
	Program->Startup();
	
	Utility->executeCommandLineArguments(argv, argc);
	Console->putLine("Test!");
	Console->putLine("Test2!");
	Program->MainLoop();

	delete Console;
	delete Utility;
	delete Files;
	delete Commands;
	delete CommandNames;

	return 0;
}