// CommandEverythingCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Loop.h"

/* dmo c++ https://guidedhacking.com/ */

#include <Windows.h>
#include <iostream>

using namespace std;

class MyHook
{
public:
	static MyHook& Instance()
	{
		static MyHook myHook;
		return myHook;
	}

	HHOOK hook;
	MSLLHOOKSTRUCT mouseStruct;
	void InstallHook();
	void UninstallHook();

	MSG msg;
	int Messsages();
};

LRESULT WINAPI MyKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);

int MyHook::Messsages()
{
	while (MyHook::Instance().msg.message != WM_QUIT)
	{
		if (PeekMessage(&MyHook::Instance().msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&MyHook::Instance().msg);
			DispatchMessage(&MyHook::Instance().msg);
		}
		Sleep(1);
	}
	MyHook::Instance().UninstallHook();
	return (int)MyHook::Instance().msg.wParam;
}

void MyHook::InstallHook()
{
	if (!(MyHook::Instance().hook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyboardCallback, NULL, GetCurrentThreadId())))
	{
		Console->WriteLine("Could not install keyboard hook.", 3);
		ostringstream stream;
		stream << GetLastError();
		string str = "Error: ";
		str.append(stream.str());
		Console->WriteLine(&str, 3);
	}
	else
	{
		Console->WriteLine("Installed hook successfully", 6);
	}
}

void MyHook::UninstallHook()
{
	UnhookWindowsHookEx(MyHook::Instance().hook);
}

LRESULT WINAPI MyKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* pMouseStruct = (KBDLLHOOKSTRUCT *)lParam;
	printf("Traffic");

	if (nCode >= 0)
	{
		if (pMouseStruct != NULL)
		{
		}

		switch (wParam)
		{
		case VK_TAB:
			printf_s("TAB \n");
			break;
		}
	}
	return CallNextHookEx(MyHook::Instance().hook, nCode, wParam, lParam);
}

int main(int argc, char* argv[])
{
	MyHook::Instance().InstallHook();
	Loop* Program = new Loop();
	Program->Startup();
	Program->FreeUpMemory();
	Program->MainLoop();

	delete ToDelete;
	delete Console;
	delete Utility;
	delete Files;
	delete Complete;
	delete Commands;
	delete CommandNames;

	return 0;
}