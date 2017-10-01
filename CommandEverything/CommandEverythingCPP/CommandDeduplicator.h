#pragma once
class CommandDeduplicator : public ICommand
{
public:
	CommandDeduplicator();
	~CommandDeduplicator();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
};

