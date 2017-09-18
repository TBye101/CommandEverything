#pragma once

#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <AclAPI.h>

#include "ICommand.h"
#include "Filing.h"
#include "CommandHelp.h"
#include "ParsedCommand.h"
#include "CommandStart.h"
#include "CommandExit.h"
#include "CommandDefend.h"
#include "CommandList.h"
#include "CommandCd.h"
#include "CommandExecute.h"
#include "CommandDelete.h"
#include "CommandCrash.h"
#include "CommandHack.h"
#include "CommandKill.h"
#include "CommandTree.h"
#include "CommandEncrypt.h"
#include "CommandDecrypt.h"
#include "RunScript.h"
#include "CommandSpecs.h"
#include "CommandHide.h"

using namespace std;

/// <summary>
/// The main logic of the program.
/// </summary>
class Loop
{
public:
	Loop();
	~Loop();

	/// <summary>
	/// The main loop of the console.
	/// </summary>
	void MainLoop();

	/// <summary>
	/// Some startup logic.
	/// </summary>
	void Startup();

private:
	/// <summary>
	/// Adds all of the known commands at startup.
	/// </summary>
	void AddAllCommands();

	void ProtectProcess();

	/// <summary>
	/// The current or last parsed command.
	/// </summary>
	ParsedCommand* parsed;
};