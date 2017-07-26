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

	/// <summary>
	/// Tries a password.
	/// </summary>
	/// <param name="attempt"></param>
	void Attempt(char attempt[]);
	void Attempt(string attempt);

	/// <summary>
	/// Comes up with passwords, and tells Attempt() to try them.
	/// </summary>
	void Crack(char supportedChars[]);

	void makeCombinations(string prefix, string chars);
};