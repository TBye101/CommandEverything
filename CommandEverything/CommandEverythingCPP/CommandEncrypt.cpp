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
	if (Parsed->Words->size() < 3)
	{
		Console->WriteLine("Missing argument(s)!");
	}
	else
	{
		if (!Files->DoesDirectoryExist(FilePath))
		{
			//Our output stream
			ofstream encryptedFile;

			//The path to the directory the file is in
			string flPath = *FilePath;
			//flPath.append("\\");
			//Add to the path the file.
			flPath.append(Parsed->Words->at(1));

			//Get an input stream from that file.
			ifstream unencryptedFile(flPath);

			//Add a new extension to signify that it is encrypted
			flPath.append(".crypt");

			//Create a new file.
			encryptedFile.open(flPath);

			string line;
			while (std::getline(unencryptedFile, line))
			{
				encryptedFile << line;
			}
			unencryptedFile.close();
			encryptedFile.flush();
			encryptedFile.close();
		}
	}
}

string* CommandEncrypt::GetName()
{
	return new string("Encrypt");
}

string* CommandEncrypt::GetHelp()
{
	return new string("Encrypts the specified file. To use, cd your way to the directory your file is at.\r\n Then do \" encrypt (your file name here) (YourKeyHere) \" and it will encrypt the file and put it in the same directory. Don't use spaces....");
}

inline char* CommandEncrypt::EncryptChar(char* character, const char* Key)
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

	char* Encrypted = new char[length];
	while (i != length)
	{
		switch (operation)
		{
		case 0:
			Encrypted[i] = (Key[i] + character[i]);
			break;
		case 1:
			Encrypted[i] = (Key[i] - character[i]);
			break;
		case 2:
			Encrypted[i] = (Key[i] * character[i]);
			break;
		case 3:
			Encrypted[i] = (Key[i] / character[i]);
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

	return Encrypted;
}