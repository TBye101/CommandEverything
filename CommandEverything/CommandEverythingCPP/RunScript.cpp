#include "stdafx.h"
#include "RunScript.h"

RunScript::RunScript()
{
}

RunScript::~RunScript()
{
}

bool RunScript::shouldRunThisCommand(ParsedCommand* Parsed)
{
	if (Parsed->words->size() >= 2)
	{
		if (Parsed->words->at(0) == "run" && Parsed->words->at(1) == "script")
		{
			if (Parsed->words->size() == 2)
			{
				Console->WriteLine("Argument missing!");
			}
			return true;
		}
	}
	return false;
}

void RunScript::run(ParsedCommand* Parsed)
{
	FILE* scriptFile;
	register long lSize;
	register char* buffer;
	register unsigned __int64 result;

	string path = *FilePath;
	path.append(Parsed->words->at(2));

	scriptFile = fopen(path.c_str(), "rb");
	if (scriptFile != NULL)
	{
		//Obtain file size
		fseek(scriptFile, 0, SEEK_END);
		lSize = ftell(scriptFile);
		rewind(scriptFile);

		//Allocate buffer memory
		buffer = (char*)malloc(sizeof(char)*lSize);
		if (buffer == NULL)
		{
			Console->WriteLine("Memory error!");
		}

		//Read the file into the buffer.
		result = fread(buffer, 1, lSize, scriptFile);

		if (result != lSize)
		{
			Console->WriteLine("File reading error!");
		}

		//Split the buffer into separate line commands.
		vector<string> commands = Utility->split(buffer);

		bool CommandRun;
		ParsedCommand* pars;
		register unsigned __int64 length = commands.size();
		//Iterate over each "input", and run it like a command.
		//Very similar to Loop.cpp
		for (register unsigned __int64 i = 0; i < length; i++)
		{
			CommandRun = false;

			pars = new ParsedCommand(&commands[i]);

			//if there is valid input....
			if (pars->words->size() > 0)
			{
				//convert the first word into the lower case
				string first = pars->words->at(0);
				transform(first.begin(), first.end(), first.begin(), ::tolower);
				pars->words->at(0) = first;

				register unsigned __int64 length = Commands->size();

				for (register unsigned __int64 i = 0; i < length; ++i)
				{
					if (Commands->at(i)->shouldRunThisCommand(pars))
					{
						Commands->at(i)->run(pars);
						CommandRun = true;
						break;
					}
				}

				if (!CommandRun)
				{
					Console->WriteLine("Invalid Command!");
				}
			}

			delete pars;
		}

		fclose(scriptFile);
		delete buffer;
	}
	else
	{
		Console->WriteLine("Invalid file!");
	}
}

string* RunScript::getName()
{
	return new string("Run Script");
}

string* RunScript::getHelp()
{
	return new string("Runs the specified file as if the user had inputted all of the text into the terminal line by line.");
}