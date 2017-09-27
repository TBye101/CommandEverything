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
	/// Creates a directory on all drives being used (RAID).
	/// </summary>
	/// <param name="path">The path to the directory to create. Must be relative to this program's file system.</param>
	void createDir(const char* path);

	/// <summary>
	/// Creates a directory on all drives being used (RAID).
	/// </summary>
	/// <param name="path">The path to the directory to create. Must be relative to this program's file system.</param>
	void createFile(const char* path);
};