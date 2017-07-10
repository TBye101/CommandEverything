#pragma once
class CommandHelp : ICommand
{
public:
	CommandHelp();
	~CommandHelp();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);
	void Run(ParsedCommand* Parsed);
	string* GetName();
	string* GetHelp();
};

