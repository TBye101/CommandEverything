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
	Sleep(5000);
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

void CommandHack::Attempt(register char attempt[])
{
	register char next;
	register __int8 size = (sizeof attempt) / sizeof(char);
	register __int8 i = 0;

	while (i != size || attempt[i] == 0)
	{
		next = attempt[i];

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

	next = VK_RETURN;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a Unicode character
	ip.ki.wScan = next; // Which key press to simulate
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

void CommandHack::Attempt(string attempt)
{
	Console->WriteLine(&attempt);
	//register char next;
	//register int size = attempt.size();
	//register int i = 0;

	//while (i != size)
	//{
	//	next = attempt[i];

	//	ip.type = INPUT_KEYBOARD; 
	//	ip.ki.time = 0;
	//	ip.ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a Unicode character
	//	ip.ki.wScan = next; // Which key press to simulate
	//	ip.ki.wVk = 0;
	//	ip.ki.dwExtraInfo = 0;
	//	SendInput(1, &ip, sizeof(INPUT));
	//	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	//	SendInput(1, &ip, sizeof(INPUT));

	//	++i;
	//}

	//ip.type = INPUT_KEYBOARD;
	//ip.ki.time = 0;
	//ip.ki.dwFlags = KEYEVENTF_UNICODE; // Specify the key as a Unicode character
	//ip.ki.wScan = VK_RETURN; // Which key press to simulate
	//ip.ki.wVk = 0;
	//ip.ki.dwExtraInfo = 0;
	//SendInput(1, &ip, sizeof(INPUT));
	//ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	//SendInput(1, &ip, sizeof(INPUT));
}

void CommandHack::Crack()
{
	//register char attempting[128];

	//32-137 
	//Acquire common characters. 

	register unsigned __int8 i = 32;

	while (i != 128)
	{
		this->Chars[i - 32] = i;
		++i;
	}

	unsigned __int8 stringlength = 1;
	while (true)
	{
		// Keep growing till I get it right
		this->makeCombinations("", stringlength);
		stringlength++;
	}


	//Will crack 128 char password
	//96 supported chars
	//for (register __int8 ii = 0; ii < 96; ++ii)
	//{
	//	attempting[i] = supportedChars[ii];
	//	this->Attempt(attempting);
	//}

	//for (register __int8 ii = 0; ii < 96; ++ii)
	//{
	//	attempting[i] = supportedChars[ii];

	//	for (register __int8 iii = 0; iii < 96; ++iii)
	//	{
	//		attempting[i + 1] = supportedChars[iii];
	//		this->Attempt(attempting);
	//	}
	//}
}

void CommandHack::makeCombinations(string s, unsigned __int8 length)
{
	if (length == 0) // when length has been reached
	{
		this->Attempt(s); // print it out
		return;
	}

	for (register unsigned __int8 i = 0; i < 96; i++) // iterate through alphabet
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		string appended = s + this->Chars[i];
		this->makeCombinations(appended, length - 1);
	}
}
