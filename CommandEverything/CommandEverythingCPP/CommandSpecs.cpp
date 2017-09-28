#include "stdafx.h"
#include "CommandSpecs.h"

CommandSpecs::CommandSpecs()
{
}

CommandSpecs::~CommandSpecs()
{
}

bool CommandSpecs::shouldRunThisCommand(ParsedCommand * Parsed)
{
	return (Parsed->words->at(0) == "specs");
}

void CommandSpecs::run(ParsedCommand* Parsed)
{
	this->GetOSInfo();
	this->GetProcessorInfo();
}

string* CommandSpecs::getName()
{
	return new string("Specs");
}

string* CommandSpecs::getHelp()
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
		Console->WriteLine(&("Processor Type: " + *this->GetProcessorName()));
		Console->WriteLine(&("Current Mhz: " + to_string(ProcessorInformations[i].CurrentMhz)));
		Console->WriteLine(&("Mhz Limit: " + to_string(ProcessorInformations[i].MhzLimit)));
		Console->WriteLine("\r\n");
		++i;
	}
}

string* CommandSpecs::GetProcessorName()
{
	int CPUInfo[4] = { -1 };
	char CPUBrandString[0x40];
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	memset(CPUBrandString, 0, sizeof(CPUBrandString));

	// Get the information associated with each extended ID.
	for (int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string.
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	return new string(CPUBrandString);
}