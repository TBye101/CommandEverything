#include "stdafx.h"
#include "CommandHack.h"

CommandHack::CommandHack()
{
}

CommandHack::~CommandHack()
{
	delete[] this->Chars;
}

bool CommandHack::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "hack");
}
//hack aAnNrRsStTuUyY1_mMeE
void CommandHack::run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() > 1)
	{
		string valid = Parsed->Words->at(1);
		register unsigned __int64 length = valid.size();
		this->Chars = new char[length];
		this->CharsLength = length;

		register unsigned __int64 i = 0;

		while (i != length)
		{
			this->Chars[i] = valid[i];
			i++;
		}
	}
	else
	{
		this->Chars = new char[96];
		this->CharsLength = 96;
		register unsigned __int8 i = 32;
		while (i != 128)
		{
			this->Chars[i - 32] = i;
			++i;
		}
	}

	Sleep(20000);
	this->Crack();
}

string* CommandHack::getName()
{
	return new string("Hack");
}

string* CommandHack::getHelp()
{
	return new string("In 10 seconds, it tries to hack into whatever input box the user is hovering over. Optional: hack (Characters here) will use only those characters in an attack. Doesn't handle spaces.");
}

void CommandHack::Attempt(string* attempt)
{
	Console->WriteLine(attempt);
	register char next;
	register unsigned __int64 size = attempt->size();
	register unsigned __int64 i = 0;

	while (i != size)
	{
		next = attempt->at(i);
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = 0;
		ip.ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a Unicode character
		ip.ki.wScan = next; // Which key press to simulate
		ip.ki.wVk = 0;
		ip.ki.dwExtraInfo = 0;
		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));

		++i;
	}

	//Enter key
	keybd_event(VK_RETURN, '5A', 0, 0);
	keybd_event(VK_RETURN, '5A', KEYEVENTF_KEYUP, 0);

	register unsigned __int8 ab = -20;

	while (ab != this->Length)
	{
		//Backspace
		keybd_event(VK_BACK, '66', 0, 0);
		keybd_event(VK_BACK, '66', KEYEVENTF_KEYUP, 0);
		++ab;
	}
}

void CommandHack::Crack()
{
	//32-137
	//Acquire common characters.
	register unsigned __int8 i = 32;
	i = 1;
	string* Attemptter = new string("");
	while (i != 128)
	{
		this->Length = i;
		// Keep growing till I get it right
		this->makeCombinations(Attemptter, i);
		++i;
	}

	ToDelete->push_back(Attemptter);
}
void CommandHack::makeCombinations(string* s, unsigned __int8 length)
{
	if (length == 0) // when length has been reached
	{
		this->Attempt(s); // print it out
		return;
	}
	register string appended;

	for (register unsigned __int8 i = 0; i < this->CharsLength; i++) // iterate through alphabet
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		appended = *s + this->Chars[i];
		this->makeCombinations(&appended, length - 1);
	}
}