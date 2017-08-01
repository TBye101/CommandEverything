#pragma once

#include <string>
#include <Windows.h>
#include <conio.h>
#include <Psapi.h>

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
};