#include "stdafx.h"
#include "CommandDecrypt.h"


CommandDecrypt::CommandDecrypt()
{
}


CommandDecrypt::~CommandDecrypt()
{
}

bool CommandDecrypt::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "decrypt");
}

void CommandDecrypt::Run(ParsedCommand* Parsed)
{
	if (Parsed->Words->size() < 3)
	{
		Console->WriteLine("Missing argument(s)!");
	}
	else
	{
		this->Cmd = *Parsed->Words;
		//Give the thread pool the encryption command.
		TPool->enqueue(&CommandDecrypt::Go, this);
	}
}

string* CommandDecrypt::GetName()
{
	return new string("Decrypt");
}

string* CommandDecrypt::GetHelp()
{
	return new string("Decrypts the specified file. To use, cd your way to the directory your file is at.\r\n Then do \" decrypt (your file name here) (YourKeyHere) \" and it will decrypt the file and put it in the same directory. Don't use spaces....");

}

string CommandDecrypt::DecryptChar(string* character, const char* Key)
{
	//this->EncryptionKey = "";

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
			Encrypted.push_back(finishedKey.at(i) - character->at(i));
			break;
		case 1:
			Encrypted.push_back(finishedKey.at(i) + character->at(i));
			break;
		case 2:
			if (character->at(i) == 0)
			{
				Encrypted.push_back(0);
			}
			else
			{
				Encrypted.push_back(character->at(i) / finishedKey.at(i));
			}
			break;
		case 3:
				Encrypted.push_back(finishedKey.at(i) * character->at(i));
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

void CommandDecrypt::Go()
{
	clock_t start;
	double duration;
	start = clock();

	if (Files->DoesDirectoryExist(FilePath))
	{
		this->EncryptionKey = new string();

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
		flPath.append(".decr");

		//Create a new file.
		encryptedFile.open(flPath);

		register string line;
		register string encrypted;

		while (unencryptedFile >> line)
		{
			//if (unencryptedFile.eof())
			//{
			//	break;
			//}
			if (!unencryptedFile)
			{
				if (!std::cin.eof())
				{
					Console->WriteLine("Unknown error has occurred in file reading!");
					break;
				}
			}
			//decrypts the characters.
			encrypted = this->DecryptChar(&line, Cmd.at(2).c_str());

			//Spew those characters to file.
			encryptedFile << encrypted;
		}

		unencryptedFile.close();
		encryptedFile.flush();
		encryptedFile.close();

		delete this->EncryptionKey;
		Console->WriteLine("Decryption done!");
	}

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Command took: " + to_string(duration)));
}
