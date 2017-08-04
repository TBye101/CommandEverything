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
	/// The key we encrypt off of.
	/// </summary>
	string* EncryptionKey;

	/// <summary>
	/// If true, stop all threads.
	/// </summary>
	bool StopThreads = false;

	/// <summary>
	/// Encrypts a string.
	/// </summary>
	/// <param name="character"></param>
	/// <returns></returns>
	inline char* EncryptChar(char* character);

	/// <summary>
	/// Decrypts a string.
	/// </summary>
	/// <param name="character"></param>
	/// <returns></returns>
	inline char* DecryptChar(char* character);

	//SYSTEM_INFO sysinfo;
	//GetSystemInfo(&sysinfo);
	//int numCPU = sysinfo.dwNumberOfProcessors;
};