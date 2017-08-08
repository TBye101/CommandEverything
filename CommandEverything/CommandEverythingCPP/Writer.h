#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <codecvt>
#include <iostream>
#include "Filing.h"

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
	/// TODO: https://github.com/Akagi201/lwlog/issues/1 color logging
	/// Writes a line to the console.
	/// This will NOT put the string passed in up for deletion. Handle your own memory.
	/// </summary>
	/// <param name="Str"></param>
	/// <param name="LogLevel">Represents the color and label attached to the log.</param>
	void WriteLine(string* Str);
	void WriteLine(const char* Str);

	/// <summary>
	/// Logs a line, but does not write it to the console.
	/// </summary>
	/// <param name="Str"></param>
	void LogLine(string* Str);

	string* GetTime();

	// notice that IIRC on modern Windows machines if you aren't admin
	// you can't write in the root directory of the system drive;
	// you should instead write e.g. in the current directory
	ofstream Log;
};
