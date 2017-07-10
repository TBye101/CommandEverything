#pragma once

#include <Windows.h>

using namespace std;

class Filing
{
public:
	Filing();
	~Filing();

	/// <summary>
	/// Returns the directory from which this executable is running from.
	/// </summary>
	DWORD GetPathToExe();
};

