#pragma once

#include <iostream>
#include <time.h>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <iostream>

#include "Filing.h"
#include "TermColor.hpp"
#include "lwlog.h"

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
	/// Log levels:
	/// EMERG (0)
	/// ALERT (1)
	/// CRIT (2)
	/// ERR (3)
	/// WARNING (4)
	/// NOTICE (5)
	/// INFO (6)
	/// DEBUG (7)
	/// NoColoring (8)
	/// </summary>
	/// <param name="Str"></param>
	/// <param name="LogLevel">Represents the color and label attached to the log.</param>
	void WriteLine(string* Str, char LogLevel);
	void WriteLine(const char* Str, char LogLevel);

	/// <summary>
	/// Logs a line, but does not write it to the console.
	/// </summary>
	/// <param name="Str"></param>
	void LogLine(string* Str);

	const char* GetTime();

	// notice that IIRC on modern Windows machines if you aren't admin
	// you can't write in the root directory of the system drive;
	// you should instead write e.g. in the current directory
	ofstream Log;

private:

	/// <summary>
	/// Storage for a message.
	/// </summary>
	string* Msg = new string();
};
