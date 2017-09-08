#pragma once

#include <thread>
#include <processthreadsapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>
#include <algorithm>

/// <summary>
/// Defends against new programs being launched.
/// </summary>
class CommandDefend : public ICommand
{
public:
	CommandDefend();
	~CommandDefend();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
private:
	//If this is true, the defense thread will stop execution.
	bool StopThread = false;
	thread* DefenseThread;

	/// <summary>
	/// A list of programs not to kill.
	/// </summary>
	unsigned int AllowedProcesses[1024];

	/// <summary>
	/// A list of programs that we failed to kill.
	/// </summary>
	vector<unsigned int> FailedHitList;

	/// <summary>
	/// Determines if the given process exists in RunningProcesses.
	/// </summary>
	/// <returns></returns>
	bool DoesProcessExistInList(unsigned int pID, unsigned int numberOfProcesses);

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