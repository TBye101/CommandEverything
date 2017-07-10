#pragma once

#include <string>

using namespace std;

/// <summary>
/// Is a parsed command.
/// </summary>
class ParsedCommand
{
public:
	/// <summary>
	/// The constructor for the ParsedCommand class.
	/// </summary>
	/// <param name="input">The string input to parse.</param>
	ParsedCommand(string* input);
	~ParsedCommand();

	string* CommandName;
	string** OptionalParameters[];
};

