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
	this->GetProcessorInfo();
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

void CommandSpecs::GetProcessorInfo()
{
	Console->WriteLine("\r\n");
	Console->WriteLine("Processor Information: ");
	Console->WriteLine("\r\n");
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	//SysInfo->dwNumberOfProcessors
	_PROCESSOR_POWER_INFORMATION* ProcessorInformations = new _PROCESSOR_POWER_INFORMATION[SysInfo.dwNumberOfProcessors];
	CallNtPowerInformation(POWER_INFORMATION_LEVEL::ProcessorInformation, NULL, NULL, ProcessorInformations, (sizeof(_PROCESSOR_POWER_INFORMATION) * SysInfo.dwNumberOfProcessors));

	__int8 i = 0;

	while (i != SysInfo.dwNumberOfProcessors)
	{
		Console->WriteLine(&("Processor " + to_string(ProcessorInformations[i].Number)));
		Console->WriteLine(&("Current Mhz: " + to_string(ProcessorInformations[i].CurrentMhz)));
		Console->WriteLine(&("Max Mhz: " + to_string(ProcessorInformations[i].MaxMhz)));
		Console->WriteLine(&("Mhz Limit: " + to_string(ProcessorInformations[i].MhzLimit)));
		Console->WriteLine("\r\n");
		++i;
	}
}
