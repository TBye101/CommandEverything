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
	virtual bool ShouldRunThisCommand(ParsedCommand* Parsed) = 0;

	/// <summary>
	/// Runs the command.
	/// </summary>
	/// <param name="Parsed"></param>
	virtual void Run(ParsedCommand* Parsed) = 0;

	/// <summary>
	/// Returns the name of the command.
	/// </summary>
	/// <returns></returns>
	virtual string* GetName() = 0;

	/// <summary>
	/// Returns some help information about the command.
	/// </summary>
	/// <returns></returns>
	virtual string* GetHelp() = 0;
};