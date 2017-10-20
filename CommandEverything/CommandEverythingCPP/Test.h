#pragma once

#include "SHA256.h"
#include "Header Only\dirent.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

/// <summary>
/// A comparator that compares strings with a file path, but only compares the filename.
/// </summary>
class compareFileName
{
public:
	bool operator()(string path1, string path2)
	{
		unsigned __int64 oneSize = path1.size();
		unsigned __int64 twoSize = path2.size();
		register unsigned __int64 leastSize;
		register unsigned __int64 i = 0;

		//Get the size of the smaller string
		if (oneSize > twoSize || oneSize == twoSize)
		{
			leastSize = oneSize;
		}
		else
		{
			leastSize = twoSize;
		}

		while (i != leastSize)
		{
			if (path1[i] > path2[i])
			{
				return false;
			}
			if (path1[i] < path2[i])
			{
				return true;;
			}
			++i;
		}

		return true;
	}
};

class Test
{
public:
	Test();
	~Test();

	/// <summary>
	/// Call this to start the test.
	/// </summary>
	void testMethod();

	/// <summary>
	/// Holds all known data about the files found.
	/// Key: filename
	/// Value: hash
	/// </summary>
	map<string, string, compareFileName>* fileData = new map<string, string, compareFileName>();
	compareFileName* Comparator = new compareFileName();

	/// <summary>
	/// Iterates over all accessible files in the directory
	/// </summary>
	void fileIterator(char* dir);

	/// <summary>
	/// Reads the contents of the specified file, and returns them as a string.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	string readContentsOfFile(char* path);
};

