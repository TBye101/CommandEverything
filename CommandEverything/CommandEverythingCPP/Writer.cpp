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

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
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
		this->WriteLine(&message, 6);
	}

	ToDelete->push_back(file);
	ToDelete->push_back(now);
}

Writer::~Writer()
{
	this->Log.close();
}

// Log levels:
// EMERG (0)
// ALERT (1)
// CRIT (2)
// ERR (3)
// WARNING (4)
// NOTICE (5)
// INFO (6)
// DEBUG (7)
// NoColoring (8)
void Writer::WriteLine(string *Str, char LogLevel)
{
	if (Str != NULL && Str->size() > 0)
	{
		string logme = this->GetTime();
		logme.append(*Str);
		logme.append("\r\n");

		if (LogLevel != 8)
		{
			switch (LogLevel)
			{
			case 0:
				lwlog_emerg("%s", logme.c_str());
				break;
			case 1:
				lwlog_alert("%s", logme.c_str());
				break;
			case 2:
				lwlog_crit("%s", logme.c_str());
				break;
			case 3:
				lwlog_err("%s", logme.c_str());
				break;
			case 4:
				lwlog_warning("%s", logme.c_str());
				break;
			case 5:
				lwlog_notice("%s", logme.c_str());
				break;
			case 6:
				lwlog_info("%s", logme.c_str());
				break;
			case 7:
				lwlog_debug("%s", logme.c_str());
				break;
			default:
				this->WriteLine("Log level not found!", 3);
				break;
			}
		}
		else
		{
			cout << logme;
			cout.flush();
		}
		this->LogLine(Str);
	}
	else
	{
		string a = "Someone attempted to write a null or empty string to the console!";
		this->WriteLine(&a, 3);
	}
}

void Writer::WriteLine(const char* Str, char LogLevel)
{
	delete this->Msg;
	Msg = new string(Str);
	this->WriteLine(this->Msg, LogLevel);
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
	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]: ");

	ToDelete->push_back(Formatted);
	ToDelete->push_back(timeinfo);

	const char* Ret = Formatted->c_str();
	return Ret;
}