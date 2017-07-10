// CommandEverythingCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ICommand.h"
#include "cmdparser.hpp"
#include "Loop.h"

#include <vector>

using namespace std;



int main(int argc, char* argv[])
{
	Loop* Program = new Loop();
	Program->Startup();
	Program->FreeUpMemory();
	Program->MainLoop();
    return 0;
}
