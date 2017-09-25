#pragma once
/// <summary>
/// Used to create a file on all drives, and write to whichever drive is ready first.
/// </summary>
class RAIDFile
{
public:

	/// <summary>
	/// Creates a file on all drives.
	/// </summary>
	/// <param name="rootPathArray">The array containing the root path to our working instance directory.</param>
	/// <param name="fileName">The name of the file to create.</param>
	/// <param name="path">The path to the file after the working instance directory.</param>
	/// <param name="arraySize">The size of the rootPathArray.</param>
	RAIDFile(wstring* rootPathArray, unsigned __int8 arraySize, const char* path, const char* fileName);
	~RAIDFile();
};