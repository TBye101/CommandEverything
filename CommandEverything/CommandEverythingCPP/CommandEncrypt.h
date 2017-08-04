#pragma once

#include "ICommand.h"
//Solve CUDA: https://msdn.microsoft.com/en-us/library/7f0aews7.aspx

/// <summary>
/// Encrypts a file.
/// </summary>
class CommandEncrypt : public ICommand
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
	string* EncryptionKey = new string("");

	/// <summary>
	/// If true, stop all threads.
	/// </summary>
	bool StopThreads = false;

	/// <summary>
	/// Encrypts a string.
	/// </summary>
	/// <param name="Msg"></param>
	/// <param name="Key"></param>
	/// <returns></returns>
	vector<char>* EncryptChar(char* Msg, const char* Key);
};