#pragma once

#include <string>

/// <summary>
/// Hacks a input box.
/// </summary>
class CommandHack : public ICommand
{
public:
	INPUT ip;

	CommandHack();
	~CommandHack();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();

private:

	char* Chars;
	unsigned __int64 CharsLength;
	unsigned __int8 Length = 0;

	/// <summary>
	/// Tries a password.
	/// </summary>
	/// <param name="attempt"></param>
	void Attempt(string* attempt);

	/// <summary>
	/// Comes up with passwords, and tells Attempt() to try them.
	/// </summary>
	void Crack();

	void makeCombinations(string* prefix, unsigned __int8);
};