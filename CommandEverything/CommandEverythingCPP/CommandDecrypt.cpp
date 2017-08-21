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

void CommandDecrypt::DecryptChar(vector<__int16>* character, const char* Key, vector<__int16>* storage)
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
	register __int16 holder = 0;
	register string finishedKey = *this->EncryptionKey;

	while (i != length)
	{
		switch (operation)
		{
		case 0:
			holder = finishedKey.at(i);
			holder -= character->at(i);
			break;
		case 1:
			holder = finishedKey.at(i);
			holder += character->at(i);
			break;
		case 2:

			if (finishedKey.at(i) != 0)
			{
				holder = character->at(i);
				holder /= finishedKey.at(i);
			}
			else
			{
				holder = 0;
			}
			break;
		case 3:
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
		if (operation == 4)
		{
			operation = 0;
		}
	}
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

		//Add to the path the file.
		flPath.append(Cmd.at(1));

		//Get an input stream from that file.
		ifstream unencryptedFile(flPath);

		//Add a new extension to signify that it is encrypted
		flPath.append(".decrypt");

		//Create a new file.
		encryptedFile.open(flPath);

		register vector<__int16>* line = new vector<__int16>();
		register vector<__int16>* decrypted = new vector<__int16>();
		__int16 holder;
		while (unencryptedFile.read(reinterpret_cast<char *>(&holder), sizeof(holder)))
		{
			line->push_back(holder);
			if (!unencryptedFile)
			{
				if (!std::cin.eof())
				{
					Console->WriteLine("Unknown error has occurred in file reading!");
					break;
				}
			}
			//Encrypt the characters.
			this->DecryptChar(line, Cmd.at(2).c_str(), decrypted);

			register unsigned __int64 length = decrypted->size();
			for (register unsigned __int16 i = 0; i < length; i++)
			{
				encryptedFile << decrypted->at(i);
			}
			decrypted->clear();
		}

		unencryptedFile.close();
		encryptedFile.flush();
		encryptedFile.close();

		delete this->EncryptionKey;
		delete decrypted;
		Console->WriteLine("Encryption done!");
	}

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	Console->WriteLine(&("Command took: " + to_string(duration)));
}
