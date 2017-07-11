#include "stdafx.h"
#include "ParsedCommand.h"

ParsedCommand::ParsedCommand(string *Input)
{
	//The size of the string.
	register unsigned int si = Input->size();

	if (si != 0)
	{
		string buf; // Have a buffer string
		stringstream ss(*Input); // Insert the string into a stream

		while (ss >> buf)
		{
			this->Words->push_back(&buf);
		}
	}
}

ParsedCommand::~ParsedCommand()
{
	ToDelete->push_back(this->Words);
}