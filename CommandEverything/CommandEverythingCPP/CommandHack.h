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

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();

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