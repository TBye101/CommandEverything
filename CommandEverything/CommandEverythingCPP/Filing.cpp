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
	char szTest[10];
	sprintf_s(szTest, "%d", ExePath);

	CreateDirectory(Utility->StringToWString(*LogDirectoryPath).c_str(), NULL);
}

string* Filing::GetPathToExe()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	ostringstream os;
	os << GetModuleFileNameW(hModule, path, MAX_PATH);
	return &os.str();
}

string* Filing::GetLogDirectoryPath()
{
	string directory;
	const size_t last_slash_idx = ExePath->rfind('\\');
	if (string::npos != last_slash_idx)
	{
		directory = ExePath->substr(0, last_slash_idx);
		directory.append("\\Logs");
	}
	else
	{
		throw new exception("Error! Directory not found from path!");
	}
	return &directory;
}
