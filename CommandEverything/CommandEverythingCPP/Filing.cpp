#include "stdafx.h"
#include "Filing.h"

Filing::Filing()
{
}

Filing::~Filing()
{
	delete this->ExePath;
	delete this->LogDirectoryPath;
}

void Filing::Startup()
{
	CreateDirectory(LogDirectoryPath->c_str(), NULL);
}

bool Filing::DoesDirectoryExist(string* Path)
{
	DWORD ftyp = GetFileAttributesA(Path->c_str());

	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		Console->WriteLine("Invalid path!");
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
		Console->WriteLine("Error! NullPointerException!");
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