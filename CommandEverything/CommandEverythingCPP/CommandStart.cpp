#include "stdafx.h"
#include "CommandStart.h"


CommandStart::CommandStart()
{
}


CommandStart::~CommandStart()
{
}

bool CommandStart::ShouldRunThisCommand(ParsedCommand *Parsed)
{
	return (Parsed->Words->at(0) == "start");
}

void CommandStart::Run(ParsedCommand *Parsed)
{
	//https://stackoverflow.com/questions/15435994/how-do-i-open-an-exe-from-another-c-exe

	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(Files->ExePath->c_str(),   // the path
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0x00000010,              // Creation flags: Open in new window
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

string * CommandStart::GetName()
{
	return new string("Start");
}

string * CommandStart::GetHelp()
{
	return new string("Starts a new instance of this command line.");
}
