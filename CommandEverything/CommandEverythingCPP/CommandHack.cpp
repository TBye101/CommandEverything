#include "stdafx.h"
#include "CommandHack.h"


CommandHack::CommandHack()
{
}


CommandHack::~CommandHack()
{
}

bool CommandHack::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "hack");
}

void CommandHack::Run(ParsedCommand* Parsed)
{
	Sleep(10000);
	this->Crack();
}

string* CommandHack::GetName()
{
	return new string("Hack");
}

string* CommandHack::GetHelp()
{
	return new string("In 20 seconds, it tries to hack into whatever input box the user is hovering over.");
}

void CommandHack::Attempt(string* attempt)
{
	register char next;
	register unsigned int size = attempt->size();
	register unsigned int i = 0;

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
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release1*
		SendInput(1, &ip, sizeof(INPUT));

		++i;
	}

	keybd_event(VK_RETURN, '5A', 0, 0);
	keybd_event(VK_RETURN, '5A', KEYEVENTF_KEYUP, 0);

	register unsigned __int8 ab = 0;

	while (ab != this->Length)
	{
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
	while (i != 128)
	{
		this->Chars[i - 32] = i;
		++i;
	}

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

	for (register unsigned __int8 i = 0; i < 96; i++) // iterate through alphabet
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		appended = *s + this->Chars[i];
		this->makeCombinations(&appended, length - 1);
	}
}