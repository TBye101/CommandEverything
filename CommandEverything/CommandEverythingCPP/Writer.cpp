#include "stdafx.h"
#include "Writer.h"

Writer::Writer()
{
	string path;
	buffer[0] = '[';

	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;
	path = converter.to_bytes(*Files->currentInstanceLog);

	path.append("\\");
	path.append("Main Log");
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
		this->LogLine(Str->c_str());
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

void Writer::LogLine(const char* Str)
{
	char* time = this->GetTime();
	this->Log << time;
	this->Log << Str;
	this->Log << "\r\n";
	this->Log.flush();
}

char* Writer::GetTime()
{
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer + 1, 80, "%Y-%m-%d-%H-%M-%S]: ", timeinfo);
	return buffer;
}