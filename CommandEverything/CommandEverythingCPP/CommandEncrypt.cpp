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

inline void CommandEncrypt::EncryptChar(char* character)
{
	//Determines which operation to do.
	//0 = add, 1 = subtract, 2 = multiply, 3 = divide
	register unsigned __int8 operation = 0;
	register unsigned __int64 i = 0;

	//Length of character
	register unsigned __int64 length = strlen(character);

	//Make the message and the key the same size.
	while (this->EncryptionKey->size() < length)
	{
		this->EncryptionKey->append(*this->EncryptionKey);
	}

	const char* Key = this->EncryptionKey->c_str();
	string* Encrypted = new string();
	while (i != length)
	{
		switch (operation)
		{
		case 0:
			Encrypted->push_back(Key[i] + character[i]);
			break;
		case 1:
			character[i] = this->EncryptionKey[i];
			break;
		case 2:

			break;
		case 3:

			break;
		default:
			Console->WriteLine("Houston, we have a problem");
			break;
		}

		++i;
		++operation;
		if (operation == 4)
		{
			operation = 0;
		}
	}
}

inline void CommandEncrypt::DecryptChar(string* character)
{

}
