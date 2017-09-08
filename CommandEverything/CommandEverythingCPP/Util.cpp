#include "stdafx.h"
#include "Util.h"

wstring Util::StringToWString(const string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

string* Util::GetFileName(unsigned int pID)
{
	void* info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pID);
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameEx(info, NULL, buffer, MAX_PATH);
	wstring ws(buffer);
	string ret(ws.begin(), ws.end());

	char drive[4];
	char directory[MAX_PATH];
	char filename[MAX_PATH];
	char extension[MAX_PATH];
	_splitpath(ret.c_str(), drive, directory, filename, extension);

	CloseHandle(info);
	return new string(filename);
}

string* Util::TrimWhitespace(string *withWhitespace)
{
	string* withoutWhitespace = new string("");

	register unsigned __int64 length = withWhitespace->size();
	char letter;
	for (register unsigned __int64 i = 0; i < length; i++)
	{
		letter = withWhitespace->at(i);
		if (letter != ' ')
		{
			withoutWhitespace->append(&letter);
		}
	}

	return withoutWhitespace;
}

char* Util::ToCharStar(string* toConvert)
{
	string str = *toConvert;
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}

void Util::write_shorts(string filename, vector<short> shorts)
{
	ofstream f;

	f.open(filename, ofstream::trunc | ofstream::binary);

	for (auto s : shorts)
	{
		f.write(reinterpret_cast<char*>(&s), sizeof(s));
	}

	//f.close();
}

vector<short> Util::read_shorts(string filename)
{
	ifstream f(filename);
	vector<short> res;
	short x;

	while (f.read(reinterpret_cast<char*>(&x), sizeof(x)))
	{
		res.push_back(x);
	}
	//f.close();

	return res;
}

vector<string> Util::split(char* arr)
{
	string str = arr;
	vector<string> result;
	int beg = 0, end = 0;//begining and end of each line in the array
	while (end = str.find('\n', beg + 1))
	{
		if (end == -1)
		{
			result.push_back(str.substr(beg));
			break;
		}
		result.push_back(str.substr(beg, end - beg));
		beg = end;
	}
	return result;
}

unsigned __int64 Util::graphicCalculateFilesIn(char * name)
{
	this->FilesFound = 0;
	return this->graphicCalculateFilesIn(name, 0);
}

unsigned __int64 Util::graphicCalculateFilesIn(char* name, unsigned __int32 indent)
{
	register DIR *dir;
	register struct dirent *entry;

	if (!(dir = opendir(name)))
	{
		return FilesFound;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (ControlCPressed)
		{
			break;
		}
		if (entry->d_type == DT_DIR)
		{
			char path[MAX_PATH];
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
				continue;
			}
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			this->GraphicCalculateFilesIn(path, indent + 2);
		}
		else
		{
			FilesFound++;
		}
	}
	closedir(dir);
	return FilesFound;
}