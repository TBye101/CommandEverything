#pragma once

#include <iostream>
#include <time.h>
#include <cstdio>
#include <ctime>

using namespace std;

/// <summary>
/// Used to write stuff to the console.
/// </summary>
class Writer
{
public:
	Writer();
	~Writer();

	/// <summary>
	/// Writes a line to the console.
	/// </summary>
	/// <param name="Str"></param>
	void WriteLine(string* Str);

	const char* GetTime();

	/// <summary>
	/// The last message to leave this class.
	/// </summary>
	string* Last;
};

