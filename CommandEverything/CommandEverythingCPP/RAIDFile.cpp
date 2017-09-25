#include "stdafx.h"
#include "RAIDFile.h"

RAIDFile::RAIDFile(wstring * rootPathArray, unsigned __int8 arraySize, const char * path, const char * fileName)
{
	wstring* paths;
	memcpy(paths, rootPathArray, (sizeof(wstring) * arraySize));

}

RAIDFile::~RAIDFile()
{
}