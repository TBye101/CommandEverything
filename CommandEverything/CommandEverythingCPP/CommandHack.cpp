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
	//32-137
	//Acquire common characters.
	register char Chars[96];
	
	register int i = 32;

	while (i != 128)
	{
		Chars[i - 32] = i;
		++i;
	}


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

	while (i != size)
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

void CommandHack::Crack()
{
}
