#pragma once

#include <VersionHelpers.h>

/// <summary>
/// Puts out some computer specs for you.
/// </summary>
class CommandSpecs : public ICommand
{
public:
	CommandSpecs();
	~CommandSpecs();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
private:
	/// <summary>
	/// Writes some information about the OS to the console.
	/// </summary>
	void GetOSInfo();
};

