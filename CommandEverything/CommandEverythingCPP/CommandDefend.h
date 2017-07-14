#pragma once

#include <thread>

/// <summary>
/// Defends against new programs being launched.
/// </summary>
class CommandDefend : public ICommand
{
public:
	CommandDefend();
	~CommandDefend();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
private:
	thread* DefenseThread;

	/// <summary>
	/// Starts the commands execution.
	/// </summary>
	void DefendStart();

	/// <summary>
	/// Stops the commands execution.
	/// </summary>
	void DefendStop();

	/// <summary>
	/// Does the actual defending.
	/// </summary>
	void Defend();
};