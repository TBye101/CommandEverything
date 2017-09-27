#pragma once

/// <summary>
/// Used to change the directory we are working in. Just like CMD.
/// </summary>
class CommandCd : public ICommand
{
public:
	CommandCd();
	~CommandCd();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
private:
	/// <summary>
	/// Does some logic for when the user wants to go to the parent directory.
	/// </summary>
	void GotoParentDir();

	/// <summary>
	/// Actually cds a normal input.
	/// </summary>
	void cdNormal(ParsedCommand*);
};
