#pragma once

#include "Header Only\dirent.h"

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

	/// <summary>
	/// Converts a string to a wstring.
	/// </summary>
	/// <param name="Str"></param>
	/// <returns></returns>
	wstring StringToWString(const string& Str);

	/// <summary>
	/// Gets the name of the executable of a running process.
	/// </summary>
	string* GetFileName(unsigned int pID);

	/// <summary>
	/// Trims the whitespace from the string.
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	string* TrimWhitespace(string*);

	/// <summary>
	/// Converts a string pointer to a char pointer.
	/// Does not delete toConvert.
	/// </summary>
	/// <param name="toConvert"></param>
	/// <returns></returns>
	char* ToCharStar(string* toConvert);

	/// <summary>
	/// Writes shorts to binary file.
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="shorts"></param>
	void write_shorts(string filename, vector<short> shorts);

	/// <summary>
	/// Reads shorts from binary file.
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	vector<short> read_shorts(string filename);

	vector<string> split(char* arr);

	/// <summary>
	/// Counts the number of files in a directory graphically on screen.
	/// Also resets a class variable.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	unsigned __int64 graphicCalculateFilesIn(char* name);

private:
	ifstream* crypt;
	unsigned __int64 FilesFound = 0;

	/// <summary>
	/// Counts the number of files in a directory graphically on screen.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="indent"></param>
	/// <returns></returns>
	unsigned __int64 graphicCalculateFilesIn(char* name, unsigned __int32 indent);
};