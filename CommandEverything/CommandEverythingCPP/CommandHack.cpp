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
	//register char Chars[96];
	//
	//register int i = 32;

	//while (i != 128)
	//{
	//	Chars[i - 32] = i;
	//	++i;
	//}
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
	register int letterCounter[128];
	register char validCharacters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890!@#$%^&*()_+[]{}:;<>,./?'";
	register char bruteString[129];  // One more to null terminate
	register int nDigits = 1;
	register int maxDigits = 128;
	register int letterIndex = 0;

		for (register int i = 0; i < maxDigits; i++)
		{
			letterCounter[i] = -1;		// Initialize them all to null.
			bruteString[i] = 0;		// Null terminate the string.
		}
		bruteString[128] = 0;
		letterCounter[0] = 0;			// But we will initialize the first counter to 0.
		register int wordLength = 0;

		while (true)
		{
			// First thing we do, is update the text string with the character that changed.
			bruteString[letterIndex] = validCharacters[letterCounter[letterIndex]];
			this->Attempt(bruteString);
			// We always start incrementing the first character.
			letterIndex = 0;
			// And if it overflows, then we start walking up the letterCounter stack.
			while (++letterCounter[letterIndex] == sizeof(validCharacters) - 1)
			{
				letterCounter[letterIndex] = 0;

				// Update the text string, at the stack position.
				bruteString[letterIndex] = validCharacters[0];

				// Let's check the next position on the stack.
				letterIndex++;

				// If we are starting a new word length.  We need to update that counter.
				if (letterIndex > wordLength)
				{
					wordLength++;

					// If we have searched all characters, for all word lengths, we are done.
					if (wordLength >= maxDigits)
					{
						printf("No valid password found.\n");
					}
				}
			}
		}
}