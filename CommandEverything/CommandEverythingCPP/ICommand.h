#pragma once

#include "ParsedCommand.h"

#include <string>

using namespace std;

/// <summary>
/// The abstract class that defines some things every command needs.
/// </summary>
class ICommand
{
public:

	/// <summary>
	/// Determines if this command should be run.
	/// </summary>
	/// <param name="Parsed"></param>
	/// <returns></returns>
	virtual bool shouldRunThisCommand(ParsedCommand* Parsed) = 0;

	/// <summary>
	/// Runs the command.
	/// </summary>
	/// <param name="Parsed"></param>
	virtual void run(ParsedCommand* Parsed) = 0;

	/// <summary>
	/// Returns the name of the command.
	/// </summary>
	/// <returns></returns>
	virtual string* getName() = 0;

	/// <summary>
	/// Returns some help information about the command.
	/// </summary>
	/// <returns></returns>
	virtual string* getHelp() = 0;
};