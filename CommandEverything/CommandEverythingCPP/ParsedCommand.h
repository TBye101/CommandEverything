#pragma once

#include <string>
#include <vector>

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

	/// <summary>
	///The first word of this is the command name.
	/// Everything after that is optional parameters.
	/// </summary>
	vector<string>* words = new vector<string>();
};
