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
	wstring* ExePath = this->GetPathToExe();

	/// <summary>
	/// The path to the log folder that exists or will be created.
	/// </summary>
	wstring* LogDirectoryPath = this->GetLogDirectoryPath();

	/// <summary>
	/// The folder for all logs in this instance of this program.
	/// </summary>
	wstring* currentInstanceLog = this->getInstanceDirPath();

	/// <summary>
	/// An array that holds the path to the directory which is the root for CE on all drives.
	/// </summary>
	wstring* instanceRootDirs;

	/// <summary>
	/// The size of instanceRootDirs.
	/// </summary>
	unsigned __int8 arraySize;

	/// <summary>
	/// Kicks off some startup logic.
	/// </summary>
	void Startup();

	/// <summary>
	/// Determines if the specified directory exists.
	/// </summary>
	/// <param name="Path"></param>
	/// <returns></returns>
	bool DoesDirectoryExist(string* Path);

private:

	/// <summary>
	/// Returns the directory from which this executable is running from.
	/// </summary>
	wstring* GetPathToExe();

	/// <summary>
	/// Gets the path to the log directory.
	/// </summary>
	/// <returns></returns>
	wstring* GetLogDirectoryPath();

	/// <summary>
	/// Returns the folder for all logs this session.
	/// </summary>
	/// <returns></returns>
	wstring* getInstanceDirPath();

	/// <summary>
	/// Returns a null terminated list of all drives connected to the computer that we can use.
	/// Creates a directory driveletter:/CE/ on every drive.
	/// </summary>
	char* getDrives();

	/// <summary>
	/// Returns the current time, formatted for a file path.
	/// </summary>
	/// <returns></returns>
	wstring* getTime();


	/// <summary>
	/// Creates a directory on all drives being used (RAID).
	/// </summary>
	/// <param name="path">Path must assume that the following string will be preappended to the path: "DriveLetter:\\CE\\InstanceDir\\</param>
	void createDirRaid(const char* path);
};