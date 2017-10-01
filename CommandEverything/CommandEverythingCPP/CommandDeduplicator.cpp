#include "stdafx.h"
#include "CommandDeduplicator.h"


CommandDeduplicator::CommandDeduplicator()
{
}


CommandDeduplicator::~CommandDeduplicator()
{
}

bool CommandDeduplicator::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->words->at(0) == "deduplicate");
}

void CommandDeduplicator::run(ParsedCommand* Parsed)
{
	ofstream log = Utility->initializeNewLog("testLog");
}

string* CommandDeduplicator::getName()
{
	return new string("Deduplicate");
}

string* CommandDeduplicator::getHelp()
{
	return new string("Writes to a file all duplicates found in the file system.");
}
