#pragma once

/// <summary>
/// Encrypts a file.
/// </summary>
class CommandEncrypt
{
public:
	CommandEncrypt();
	~CommandEncrypt();

private:
	/// <summary>
	/// If true, stop all threads.
	/// </summary>
	bool StopThreads = false;

	inline char EncryptChar(char character);

	//SYSTEM_INFO sysinfo;
	//GetSystemInfo(&sysinfo);
	//int numCPU = sysinfo.dwNumberOfProcessors;
};