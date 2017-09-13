#include "stdafx.h"
#include "Writer.h"

Writer::Writer()
{
	Filing* file = new Filing();
	file->Startup();

	string path;

	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;

	path = converter.to_bytes(*file->LogDirectoryPath);

	path.append("\\");
	path.append(*this->getTime());
	path.pop_back();
	path.pop_back();
	path.append(".txt");

	this->fLog = fopen(path.c_str(), "wb");

	FILE* a = this->fLog;

	if (!fLog)
	{
		throw new exception("Can't access file!");
	}
	else
	{
		string message = "Log has initialized successfully";
		this->writeLine(&message);
	}

	//this->Log.open(path, fstream::out);

	//if (!Log)
	//{
	//	throw new exception("Can't access file!");
	//}
	//else
	//{
	//	string message = "Log has initialized successfully";
	//	this->writeLine(&message);
	//}

	//ToDelete->push_back(file);
}

Writer::~Writer()
{
	//this->Log.close();
	fclose(this->fLog);
}

void Writer::writeLine(string *Str)
{
	if (Str != NULL && Str->size() > 0)
	{
		cout << *Str;
		cout << "\r\n";
		cout.flush();
		this->LogLine(Str);
	}
	else
	{
		string a = "Someone attempted to write a null or empty string to the console!";
		this->writeLine(&a);
	}
}

void Writer::writeLine(const char* Str)
{
	string* Msg = new string(Str);
	this->writeLine(Msg);
	delete Msg;
}

void Writer::LogLine(string *Str)
{
	string a = *this->getTime();
	a.append(*Str);
	a.append("\r\n");
	const char* buffer = a.c_str();
	fwrite(buffer, sizeof(char), a.size(), this->fLog);
	//fflush(this->fLog);
	//this->Log << *this->getTime();
	//this->Log << *Str;
	//this->Log << "\r\n";
	//this->Log.flush();
}

string* Writer::getTime()
{
	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]: ");
	return Formatted;
}