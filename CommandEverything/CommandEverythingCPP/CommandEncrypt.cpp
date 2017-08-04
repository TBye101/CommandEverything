#include "stdafx.h"
#include "CommandEncrypt.h"


CommandEncrypt::CommandEncrypt()
{
}


CommandEncrypt::~CommandEncrypt()
{
}

bool CommandEncrypt::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "encrypt");
}

void CommandEncrypt::Run(ParsedCommand* Parsed)
{
}

string* CommandEncrypt::GetName()
{
	return new string("Encrypt");
}

string* CommandEncrypt::GetHelp()
{
	return new string("Encrypts the specified file. To use, cd your way to the directory your file is at.\r\n Then do \" encrypt (your file name here) \" and it will encrypt the file and put it in the same directory");
}

inline char CommandEncrypt::EncryptChar(char character)
{
	return 0;
}
