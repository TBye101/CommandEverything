#pragma once
class CommandHide : public ICommand
{
public:
	CommandHide();
	~CommandHide();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};

