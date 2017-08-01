#include "stdafx.h"
#include "Util.h"


wstring Util::StringToWString(const string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

string* Util::GetFileName(unsigned int pID)
{
	void* info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pID);
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameEx(info, NULL, buffer, MAX_PATH);
	wstring ws(buffer);
	string ret(ws.begin(), ws.end());

	char drive[4];
	char directory[MAX_PATH];
	char filename[MAX_PATH];
	char extension[MAX_PATH];
	_splitpath(ret.c_str(), drive, directory, filename, extension);

	CloseHandle(info);
	return new string(filename);
}
