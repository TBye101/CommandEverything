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
	string* now = new string(this->GetTime());
	path.append(*now);
	path.pop_back();
	path.pop_back();
	path.append(".txt");

	this->Log.open(path, fstream::out);

	if (!Log)
	{
		throw new exception("Can't access file!");
	}
	else
	{
		string message = "Log has initialized successfully";
		this->WriteLine(&message);
	}

	ToDelete->push_back(file);
	ToDelete->push_back(now);
}

Writer::~Writer()
{
	this->Log.close();
}

void Writer::WriteLine(string *Str)
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
		this->WriteLine(&a);
	}
}

void Writer::WriteLine(const char* Str)
{
	string* Msg = new string(Str);
	this->WriteLine(Msg);
	delete Msg;
}

void Writer::LogLine(string *Str)
{
	this->Log << this->GetTime();
	this->Log << *Str;
	this->Log << "\r\n";
	this->Log.flush();
}

const char* Writer::GetTime()
{
	string Formatted = "[";
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted.append(buffer);
	Formatted.append("]: ");
	delete timeinfo;
	const char* Ret = Formatted.c_str();
	return Ret;
}