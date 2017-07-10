#include "stdafx.h"
#include "ParsedCommand.h"

ParsedCommand::ParsedCommand(string *Input)
{
	//The size of the string.
	register unsigned int si = Input->size();
	//Our iterator position.
	register unsigned int i = 0;

	register char One = Input->at(0);
	//At the second loop, this is used to keep track of how many words we have processed.
	register char Two = Input->at(0);
	//Used as a boolean value, to keep track of if we are in the middle of processing a word.
	register char Three = Input->at(0);
	//If you manage to overflow this int, whatever you typed in is way to long.
	unsigned __int8 WordsFound = 0;

	for (i = 0; i < si; i++)
	{
		One = Two;
		Two = Three;
		Three = Input->at(i);

		if (One != ' ' && Two == ' ' && Three != ' ')
		{
			++WordsFound;
		}
	}

	Words = new string[WordsFound];

	//Resetting the variables
	Three = 0; //false
	//At the second loop, this is used as the start position of a word.
	unsigned int start;
	
	for (i = 0; i < si; i++)
	{
		if (WordsFound < 1)
		{
			return;
		}
		else
		{

			if (Input->at(i) != ' ')
			{
				if (Three == 0)
				{
					Three = 1; //true
					start = i;
				}
			}
			else
			{
				if (Three == 1)
				{
					Three = 0;
					Words[WordsFound - 1] = Input->substr(start, i - 1);
					--WordsFound;
				}
			}
		}
	}
}

ParsedCommand::~ParsedCommand()
{
	ToDelete->push_back(this->Words);
}