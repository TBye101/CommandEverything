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
		this->Cmd = *Parsed->Words;
		//Give the thread pool the encryption command.
		TPool->enqueue(&CommandEncrypt::Go, this);
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

string CommandEncrypt::EncryptChar(string* character, const char* Key)
{
	//Determines which operation to do.
	//0 = add, 1 = subtract, 2 = multiply, 3 = divide
	register unsigned __int8 operation = 0;
	register unsigned __int64 i = 0;

	//Length of character
	register unsigned __int64 length = character->size();

	//Make the message and the key the same size.
	while (this->EncryptionKey->size() < length)
	{
		this->EncryptionKey->append(Key);
	}

	register string finishedKey = *this->EncryptionKey;
	register string Encrypted = "";
	while (i != length)
	{
		switch (operation)
		{
		case 0:
			Encrypted.push_back(finishedKey.at(i) + character->at(i));
			break;
		case 1:
			Encrypted.push_back(finishedKey.at(i) - character->at(i));
			break;
		case 2:
			Encrypted.push_back(finishedKey.at(i) * character->at(i));
			break;
		case 3:
			Encrypted.push_back(finishedKey.at(i) / character->at(i));
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

	//delete finishedKey;
	return Encrypted;
}

void CommandEncrypt::Go()
{
	if (Files->DoesDirectoryExist(FilePath))
	{
		//Our output stream
		ofstream encryptedFile;

		//The path to the directory the file is in
		string flPath = *FilePath;
		//flPath.append("\\");
		//Add to the path the file.
		flPath.append(Cmd.at(1));

		//Get an input stream from that file.
		ifstream unencryptedFile(flPath);

		//Add a new extension to signify that it is encrypted
		flPath.append(".crypt");

		//Create a new file.
		encryptedFile.open(flPath);

		register string line;
		register string encrypted;
		while (std::getline(unencryptedFile, line))
		{
			//Encrypt the characters.
			encrypted = this->EncryptChar(&line, Cmd.at(2).c_str());

			//Spew those characters to file.
			encryptedFile << encrypted;
			encryptedFile.flush();

			//delete encryptedChars;
			//delete encrypted;
		}
		unencryptedFile.close();
		encryptedFile.flush();
		encryptedFile.close();

		delete this->EncryptionKey;

		Console->WriteLine("Encryption done!");
	}
}
