#include "stdafx.h"
#include "Writer.h"


Writer::Writer()
{
}


Writer::~Writer()
{
}

void Writer::WriteLine(string *Str)
{
	time_t ti = time(NULL);
	this->Last = new string("");
	this->Last->append(this->GetTime());
	this->Last->append(*Str);
	cout << this->Last;
}

const char * Writer::GetTime()
{
	string* Formatted = new string("[");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	Formatted->append(buffer);
	Formatted->append("]");

	ToDelete->push_back(Formatted);
	ToDelete->push_back(timeinfo);

	const char* Ret = Formatted->c_str();
	return Ret;
}