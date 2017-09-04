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

void CommandEncrypt::EncryptChar(string* character, const char* Key, vector<__int16>* storage)
{
	//this->EncryptionKey = "";

	//Determines which operation to do.
	//0 = add, 1 = subtract, 2 = multiply, 3 = divide
	register unsigned __int8 operation = 0;
	register unsigned __int64 i = 0;

	//Length of character
	register unsigned __int64 length = character->size();

	//Make the message and the key the same size.
	while (this->DecryptionKey->size() < length)
	{
		this->DecryptionKey->append(Key);
	}
	register __int16 holder = 0;
	register string finishedKey = *this->DecryptionKey;

	while (i != length)
	{
		switch (operation)
		{
		case 0:
			holder = finishedKey.at(i);
			holder += character->at(i);
			break;
		case 1:
			holder = finishedKey.at(i);
			holder -= character->at(i);
			break;
		case 2:
			holder = finishedKey.at(i);
			holder *= character->at(i);
			break;
		default:
			Console->WriteLine("Houston, we have a problem");
			break;
		}
		storage->push_back(holder);

		++i;
		++operation;
		if (operation == 3)
		{
			operation = 0;
		}
	}
}

void CommandEncrypt::Go()
{
	clock_t start;
	double duration;
	start = clock();

	if (Files->DoesDirectoryExist(FilePath))
	{
		this->DecryptionKey = new string();

		//Our output stream
		ofstream encryptedFile;

		//The path to the directory the file is in
		string flPath = *FilePath;

		//Add to the path the file.
		flPath.append(Cmd.at(1));

		//Get an input stream from that file.
		ifstream unencryptedFile(flPath);

		//Add a new extension to signify that it is encrypted
		flPath.append(".crypt");

		//Create a new file.
		encryptedFile.open(flPath);

		register string line;
		register vector<short>* encrypted = new vector<short>();

		while (unencryptedFile >> line)
		{
			if (!unencryptedFile)
			{
				if (!std::cin.eof())
				{
					Console->WriteLine("Unknown error has occurred in file reading!");
					break;
				}
			}
			//line += "\r\n";
				//Encrypt the characters.
			this->EncryptChar(&line, Cmd.at(2).c_str(), encrypted);

			Utility->write_shorts(flPath, *encrypted);
			encrypted->clear();
		}

		unencryptedFile.close();
		encryptedFile.flush();
		encryptedFile.close();

		delete this->DecryptionKey;
		delete encrypted;
		Console->WriteLine("Encryption done!");
	}

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Command took: " + to_string(duration)));
}