#pragma once

#include <thread>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>

/// <summary>
/// Defends against new programs being launched.
/// </summary>
class CommandDefend : public ICommand
{
public:
	CommandDefend();
	~CommandDefend();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
private:
	//If this is true, the defense thread will stop execution.
	bool StopThread = false;
	thread* DefenseThread;

	/// <summary>
	/// A list of programs not to kill.
	/// </summary>
	DWORD AllowedProcesses[1024];
	
	/// <summary>
	/// Determines if the given process exists in RunningProcesses.
	/// </summary>
	/// <returns></returns>
	bool DoesProcessExistInList(DWORD pID, DWORD numberOfProcesses);

	/// <summary>
	/// Starts the commands execution.
	/// </summary>
	void DefendStart();

	/// <summary>
	/// Stops the commands execution.
	/// </summary>
	void DefendStop();

	/// <summary>
	/// Does the actual defending.
	/// </summary>
	void Defend();
};