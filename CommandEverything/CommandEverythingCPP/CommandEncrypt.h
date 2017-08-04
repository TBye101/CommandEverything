#pragma once

#include "ICommand.h"
//Solve CUDA: https://msdn.microsoft.com/en-us/library/7f0aews7.aspx

/// <summary>
/// Encrypts a file.
/// </summary>
class CommandEncrypt
{
public:
	CommandEncrypt();
	~CommandEncrypt();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();

private:
	/// <summary>
	/// If true, stop all threads.
	/// </summary>
	bool StopThreads = false;

	inline char EncryptChar(char character);

	//SYSTEM_INFO sysinfo;
	//GetSystemInfo(&sysinfo);
	//int numCPU = sysinfo.dwNumberOfProcessors;
};