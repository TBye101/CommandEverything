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
	string* NextAttempt = new string("");
	string* Appended = new string();

	char Chars[96];

	/// <summary>
	/// Tries a password.
	/// </summary>
	/// <param name="attempt"></param>
	void Attempt(string* attempt, unsigned int length);

	/// <summary>
	/// Comes up with passwords, and tells Attempt() to try them.
	/// </summary>
	void Crack();

	void makeCombinations(string* prefix, unsigned __int8);
};