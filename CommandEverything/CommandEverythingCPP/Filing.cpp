#include "stdafx.h"
#include "Filing.h"

Filing::Filing()
{
	this->Startup();
}

Filing::~Filing()
{
	delete this->ExePath;
	delete this->LogDirectoryPath;
}

void Filing::Startup()
{
	CreateDirectory(LogDirectoryPath->c_str(), NULL);

	//Session log directory.
	CreateDirectory(this->currentInstanceLog->c_str(), NULL);
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

	time_t rawtime;
	tm* timeinfo;
	char buffer[81];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S]", timeinfo);

	int len;
	int slength = 82;
	len = MultiByteToWideChar(CP_ACP, 0, buffer, slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, buffer, slength, buf, len);
	wstring r(buf);

	dir->append(r);

	delete[] buf;
	return dir;
}
