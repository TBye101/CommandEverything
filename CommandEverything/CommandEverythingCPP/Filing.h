#pragma once

#include <Windows.h>
#include <sstream>

using namespace std;

class Filing
{
public:
	Filing();
	~Filing();

	/// <summary>
	/// The path to this exe.
	/// </summary>
	string* ExePath = this->GetPathToExe();

	/// <summary>
	/// The path to the log folder that exists or will be created.
	/// </summary>
	string* LogDirectoryPath = this->GetLogDirectoryPath();

	/// <summary>
	/// Kicks off some startup logic.
	/// </summary>
	void Startup();

private:

	/// <summary>
	/// Returns the directory from which this executable is running from.
	/// </summary>
	string* GetPathToExe();

	/// <summary>
	/// Gets the path to the log directory.
	/// </summary>
	/// <returns></returns>
	string* GetLogDirectoryPath();
};

