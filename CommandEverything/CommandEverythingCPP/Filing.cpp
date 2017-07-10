#include "stdafx.h"
#include "Filing.h"


Filing::Filing()
{
}


Filing::~Filing()
{
}

DWORD Filing::GetPathToExe()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	return GetModuleFileNameW(hModule, path, MAX_PATH);
}