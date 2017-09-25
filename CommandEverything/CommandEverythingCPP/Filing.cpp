#include "stdafx.h"
#include "Filing.h"

Filing::Filing()
{
	this->Startup();
	this->getDrives();
}

Filing::~Filing()
{
	delete this->ExePath;
	delete this->LogDirectoryPath;
}

void Filing::Startup()
{
	bool a = CreateDirectory(LogDirectoryPath->c_str(), NULL);

	//Session log directory.
	bool b = CreateDirectory(this->currentInstanceLog->c_str(), NULL);

	const char* driveLetters = this->getDrives();
	wstring* time = this->getTime();

	//Get size of driveLetters
	register unsigned __int8 length = 0;

	while (driveLetters[length] != '\n')
	{
		++length;
	}

	this->instanceRootDirs = new wstring[length];
	this->arraySize = length;

	register unsigned __int8 i = 0;
	while (i != length)
	{
		this->instanceRootDirs[i] = driveLetters[i];
		this->instanceRootDirs[i].append(L":\\CE\\");
		this->instanceRootDirs[i].append(*time);
		this->instanceRootDirs[i].append(L"\\");
		bool c = CreateDirectory(this->instanceRootDirs[i].c_str(), NULL);

		if (!c)
		{
			cout << "Error! Unable to create directory! This could cause critical errors.";
		}

		++i;
	}
	delete driveLetters;
}

bool Filing::DoesDirectoryExist(string* Path)
{
	DWORD ftyp = GetFileAttributesA(Path->c_str());

	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		cout << "Invalid path!\r\n";
		return false;  //something is wrong with your path!
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;   // this is a directory!
	}

	return false;    // this is not a directory!
}

wstring* Filing::GetPathToExe()
{
#ifdef UNICODE
	TCHAR ownPth[260];
#else
	char ownPth[MAX_Path];
#endif // UNICODE

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
		return new wstring(ownPth);
	}
	else
	{
		cout << "Error! NullPointerException!";
		return NULL;
	}
}

wstring* Filing::GetLogDirectoryPath()
{
	wstring *directory;
	const size_t last_slash_idx = ExePath->rfind('\\');
	if (string::npos != last_slash_idx)
	{
		directory = new wstring(ExePath->substr(0, last_slash_idx));
		directory->append(L"\\Logs");
	}
	else
	{
		Console->WriteLine("Error! Directory not found from path!");
	}
	return directory;
}

wstring* Filing::getInstanceDirPath()
{
	wstring* dir = new wstring(*this->LogDirectoryPath);
	dir->append(L"\\");

	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]");
	wstring time(Formatted->begin(), Formatted->end());
	dir->append(time);

	delete Formatted;
	return dir;
}

char* Filing::getDrives()
{
	unsigned long mydrives = 100; // buffer length
	wchar_t lpBuffer[100]; // buffer for drive string storage
	unsigned long test = GetLogicalDriveStrings(mydrives, lpBuffer); //Gets size of drive string.
	unsigned __int8 drives = test / 4; //Gets the number of drives.
	char* driveLetters = new char[drives];//We will hold the actual drive letters here.
	char* strbuffer = new char[1];//Holds the 1 character drive letter here.
	wstring drive = L"";
	string drivePath;
	unsigned __int8 drivesUsable = 0;
	int dir;//Holds whether the create directory succeeded or not.
	unsigned __int64 error;//Holds the last error code fetched.

	for (unsigned __int8 i = 0; i < drives; i++)
	{
		drive = L"";
		wctomb(strbuffer, lpBuffer[i * 4]);
		drivePath = string(strbuffer, 1);
		drivePath.append(":\\CE");
		//Check drive accessibility here
		drive.append(Utility->stringToWString(drivePath));
		dir = CreateDirectory(drive.c_str(), NULL);

		error = GetLastError();
		if (dir != 0 || error  == ERROR_ALREADY_EXISTS)
		{
			{
				driveLetters[drivesUsable] = strbuffer[0];
				++drivesUsable;
			}
		}
	}
	driveLetters[drivesUsable] = '\n';
	delete strbuffer;

	return driveLetters;
}

wstring* Filing::getTime()
{
	wstring* dir = new wstring();
	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]");
	wstring time(Formatted->begin(), Formatted->end());
	dir->append(time);

	delete Formatted;
	return dir;
}

void Filing::createDirRaid(const char* path)
{
	register unsigned __int8 i = 0;

	while (i != this->arraySize)
	{
		
	}
}