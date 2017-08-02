// CommandEverythingCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Loop.h"

/* dmo c++ https://guidedhacking.com/ */

using namespace std;


BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		ControlCPressed = true;
		return(TRUE);
	case CTRL_CLOSE_EVENT:
		Beep(600, 200);
		printf("Ctrl-Close event\n\n");
		return(TRUE);

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		Beep(900, 200);
		printf("Ctrl-Break event\n\n");
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		Beep(1000, 200);
		printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		Beep(750, 500);
		printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
	{
		Console->WriteLine("!Error!: Could not set event handler.");
	}
	Loop* Program = new Loop();
	Program->Startup();
	Program->FreeUpMemory();
	Program->MainLoop();

	delete ToDelete;
	delete Console;
	delete Utility;
	delete Files;
	delete Commands;
	delete CommandNames;

	return 0;
}