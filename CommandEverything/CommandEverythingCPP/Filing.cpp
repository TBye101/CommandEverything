#include "stdafx.h"
#include "Filing.h"


Filing::Filing()
{
}


Filing::~Filing()
{
	ToDelete->push_back(this->ExePath);
	ToDelete->push_back(this->LogDirectoryPath);
}

void Filing::Startup()
{
	CreateDirectory(LogDirectoryPath->c_str(), NULL);
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
		throw new exception("Error! NullPointerException!");
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
		throw new exception("Error! Directory not found from path!");
	}
	return directory;
}
