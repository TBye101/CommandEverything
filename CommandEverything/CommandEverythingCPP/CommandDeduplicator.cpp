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
	TPool->enqueue(&CommandDeduplicator::seperateThread, this);
}

string* CommandDeduplicator::getName()
{
	return new string("Deduplicate");
}

string* CommandDeduplicator::getHelp()
{
	return new string("Writes to a file all duplicates found in the file system.");
}

void CommandDeduplicator::seperateThread()
{
	clock_t start;
	double duration;
	start = clock();

	this->log = Utility->initializeNewLog("deduplicator");

	

	this->log.close();

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Deduplicator took: " + to_string(duration)));
}

void CommandDeduplicator::fileIterator()
{
}
