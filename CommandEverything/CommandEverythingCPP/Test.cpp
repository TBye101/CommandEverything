#include "stdafx.h"
#include "Test.h"


Test::Test()
{
}


Test::~Test()
{
}

void Test::testMethod()
{
	this->fileIterator("C:/");
}

void Test::fileIterator(char* name)
{
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(name)))
	{
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (ControlCPressed)
		{
			break;
		}
		if (entry->d_type == DT_DIR)
		{
			char path[MAX_PATH];
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
				continue;
			}
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			this->fileIterator(path);
		}
		else
		{
			try
			{
				//File found. Work on it.
				//Get the hash of the file.
				string sha = sha256(this->readContentsOfFile(entry->d_name));
				this->fileData->insert(pair<string, string>(entry->d_name, sha));
				if (fileData->size() > 2000000)
				{
					int i = 0;
					int e = 0;
					string a = "";
				}
			}
			catch (exception e)
			{
				cout << e.what();
			}

		}
	}
	closedir(dir);
}

string Test::readContentsOfFile(char * path)
{
	ifstream fl;
	fl.open(path, ofstream::in);

	string line = "";
	string ret = "";
	unsigned __int64 i = 0;
	while (getline(fl, line))
	{
		ret.append(line);
	}

	return ret;
}
