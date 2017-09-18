#include "stdafx.h"
#include "CommandHide.h"


CommandHide::CommandHide()
{
}


CommandHide::~CommandHide()
{
}

bool CommandHide::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "hide");
}

void CommandHide::run(ParsedCommand* Parsed)
{
	HWND console = GetConsoleWindow();
	ShowWindow(console, 0);
}

string* CommandHide::getName()
{
	return new string("Hide");
}

string* CommandHide::getHelp()
{
	return new string("Hides the console.");
}
