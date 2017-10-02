#include "stdafx.h"
#include "Util.h"

Util::Util()
{
	this->buffer[0] = '[';
}

wstring Util::stringToWString(const string & s)
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

string* Util::getFileName(unsigned int pID)
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

string* Util::trimWhitespace(string *withWhitespace)
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

char* Util::toCharStar(string* toConvert)
{
	string str = *toConvert;
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}

void Util::writeShorts(string filename, vector<short> shorts)
{
	ofstream f;

	f.open(filename, ofstream::trunc | ofstream::binary);

	for (auto s : shorts)
	{
		f.write(reinterpret_cast<char*>(&s), sizeof(s));
	}

	//f.close();
}

vector<short> Util::readShorts(string filename)
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

unsigned __int64 Util::graphicCalculateFilesIn(const char * name)
{
	this->FilesFound = 0;
	return this->graphicCalculateFilesIn(name, 0);
}

void Util::getFilenameFromPath(string* Path)
{
	// Remove directory if present.
	// Do this before extension removal incase directory has a period character.
	const size_t last_slash_idx = Path->find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		Path->erase(0, last_slash_idx + 1);
	}
}

void Util::executeCommandLineArguments(char* argv[], int argC)
{
	register unsigned __int64 i = 0;
	bool CommandRun = false;
	ParsedCommand* parsed;
	string expendable;

	if (argC > 1)
	{
		Console->WriteLine("Command line arguments:");
		while (i != argC)
		{
			if (i != 0)
			{
				Console->WriteLine(argv[i]);
				CommandRun = false;
				expendable = string(argv[i]);
				Console->LogLine(expendable.c_str());

				parsed = new ParsedCommand(&expendable);

				//if there is valid input....
				if (parsed->words->size() > 0)
				{
					//convert the first word into the lower case
					string first = parsed->words->at(0);
					transform(first.begin(), first.end(), first.begin(), ::tolower);
					parsed->words->at(0) = first;

					register unsigned __int64 length = Commands->size();
					register unsigned __int64 ii = 0;
					for (ii = 0; ii < length; ++ii)
					{
						if (Commands->at(ii)->shouldRunThisCommand(parsed))
						{
							Commands->at(ii)->run(parsed);
							CommandRun = true;
							break;
						}
					}

					if (!CommandRun)
					{
						Console->WriteLine("Invalid Command!");
					}
					delete parsed;
				}

			}
			++i;
		}
	}
}

char* Util::getTime()
{
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer + 1, 80, "%Y-%m-%d-%H-%M-%S] ", timeinfo);
	return buffer;
}

void Util::logLine(ofstream* log, const char* msg)
{
	*log<< this->getTime();
	*log << msg;
	*log << "\r\n";
}

ofstream Util::initializeNewLog(char* filename)
{
	string path;
	ofstream newLog;
	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;
	path = converter.to_bytes(*Files->currentInstanceLog);

	path.append("\\");
	path.append(Utility->getTime());
	path.append(filename);
	path.append(".txt");

	newLog.open(path, fstream::out);

	if (!newLog)
	{
		cout << "Can't access file!\r\n";
	}

	return newLog;
}

fstream Util::initializeNewLogAndReader(char* filename)
{
	string path;
	fstream newLog;
	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;
	path = converter.to_bytes(*Files->currentInstanceLog);

	path.append("\\");
	path.append(Utility->getTime());
	path.append(filename);
	path.append(".txt");

	newLog.open(path);

	if (!newLog)
	{
		cout << "Can't access file!\r\n";
	}

	return newLog;
}

unsigned __int64 Util::graphicCalculateFilesIn(const char* name, unsigned __int32 indent)
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
			this->graphicCalculateFilesIn(path, indent + 2);
		}
		else
		{
			FilesFound++;
		}
	}
	closedir(dir);
	return FilesFound;
}