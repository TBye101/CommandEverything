#pragma once
#include "ICommand.h"
#include "Filing.h"
#include "CommandHelp.h"
#include "ParsedCommand.h"
#include <Windows.h>
#include <algorithm>
#include "CommandStart.h"
#include <Windows.h>
#include <stdio.h>

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
	/// Frees up some memory, that we were waiting to delete.
	/// </summary>
	void FreeUpMemory();

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

	/// <summary>
	/// The last user input.
	/// </summary>
	string Input;

	/// <summary>
	/// The current or last parsed command.
	/// </summary>
	ParsedCommand* parsed;


	/// <summary>
	/// Receives keyboard hook information.
	/// </summary>
	/// <param name="nCode"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Sets the keyboard hook.
	/// </summary>
	void SetHook();

	/// <summary>
	/// Releases the keyboard hook.
	/// </summary>
	void ReleaseHook();
	
	// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
	// or you will get problems since every function uses this variable.
	HHOOK _hook;

	// This struct contains the data received by the hook callback. As you see in the callback function
	// it contains the thing you will need: vkCode = virtual key code.
	KBDLLHOOKSTRUCT kbdStruct;
};