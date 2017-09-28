#pragma once

#include "Header Only\dirent.h"
#include "RunScript.h"
#include "ParsedCommand.h"

#include <string>
#include <Windows.h>
#include <conio.h>
#include <Psapi.h>
#include <vector>

using namespace std;

/// <summary>
/// Used to do some utility functions.
/// </summary>
class Util
{
public:

	Util();

	/// <summary>
	/// Converts a string to a wstring.
	/// </summary>
	/// <param name="Str"></param>
	/// <returns></returns>
	wstring stringToWString(const string& Str);

	/// <summary>
	/// Gets the name of the executable of a running process.
	/// </summary>
	string* getFileName(unsigned int pID);

	/// <summary>
	/// Trims the whitespace from the string.
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	string* trimWhitespace(string*);

	/// <summary>
	/// Converts a string pointer to a char pointer.
	/// Does not delete toConvert.
	/// </summary>
	/// <param name="toConvert"></param>
	/// <returns></returns>
	char* toCharStar(string* toConvert);

	/// <summary>
	/// Writes shorts to binary file.
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="shorts"></param>
	void writeShorts(string filename, vector<short> shorts);

	/// <summary>
	/// Reads shorts from binary file.
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	vector<short> readShorts(string filename);

	vector<string> split(char* arr);

	/// <summary>
	/// Counts the number of files in a directory graphically on screen.
	/// Also resets a class variable.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	unsigned __int64 graphicCalculateFilesIn(char* name);

	/// <summary>
	/// Returns the filename including the extension from a path.
	/// </summary>
	/// <param name="Path"></param>
	/// <returns></returns>
	void getFilenameFromPath(string* Path);

	/// <summary>
	/// Used to launch command(s) via command line parameters.
	/// </summary>
	/// <param name="argv"></param>
	/// <param name="argC"></param>
	void executeCommandLineArguments(char* argv[], int argC);
	
	/// <summary>
	/// Gets the current time.
	/// Don't delete the address pointed to by this methods return address. It is reused.
	/// </summary>
	/// <returns></returns>
	char* getTime();

private:
	ifstream* crypt;
	unsigned __int64 FilesFound = 0;

	time_t rawtime;
	tm* timeinfo;
	char* buffer = new char[81];

	/// <summary>
	/// Counts the number of files in a directory graphically on screen.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="indent"></param>
	/// <returns></returns>
	unsigned __int64 graphicCalculateFilesIn(char* name, unsigned __int32 indent);
};