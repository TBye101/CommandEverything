#include "stdafx.h"
#include "CommandSpecs.h"


CommandSpecs::CommandSpecs()
{
}


CommandSpecs::~CommandSpecs()
{
}

bool CommandSpecs::ShouldRunThisCommand(ParsedCommand * Parsed)
{
	return (Parsed->Words->at(0) == "specs");
}

void CommandSpecs::Run(ParsedCommand* Parsed)
{
	this->GetOSInfo();
}

string* CommandSpecs::GetName()
{
	return new string("Specs");
}

string* CommandSpecs::GetHelp()
{
	return new string("Writes to the console some detailed information about your computer.");
}

void CommandSpecs::GetOSInfo()
{
	__int64 major = 0;
	__int64 minor = 0;
	__int64 serviceMajor = 0;

	//Determine major version number.
	while (IsWindowsVersionOrGreater(major, minor, serviceMajor))
	{
		++major;
	}
	--major;

	//Determine minor version number.
	while (IsWindowsVersionOrGreater(major, minor, serviceMajor))
	{
		++minor;
	}
	--minor;

	//Determine service pack major version number.
	while (IsWindowsVersionOrGreater(major, minor, serviceMajor))
	{
		serviceMajor++;
	}
	serviceMajor--;

	Console->WriteLine("OS Info: ");
	Console->WriteLine(&("Major: " + to_string(major)));
	Console->WriteLine(&("Minor: " + to_string(minor)));
	Console->WriteLine(&("Service: " + to_string(serviceMajor)));
}
