#include "stdafx.h"
#include "Loop.h"


Loop::Loop()
{
}


Loop::~Loop()
{
	ToDelete->push_back(this->parsed);
}


/// <summary>
/// The main logic loop of the console.
/// </summary>
void Loop::MainLoop()
{
	while (true)
	{
		try
		{
			this->Input = *Utility->GetLine();
			Console->LogLine(&Input);

			std::transform(Input.begin(), Input.end(), Input.begin(), ::tolower);

			parsed = new ParsedCommand(&Input);
			//if there is valid input....
			if (parsed->Words->size() > 0)
			{

				register size_t length = Commands->size();

				for (register size_t i = 0; i < length; ++i)
				{
					if (Commands->at(i)->ShouldRunThisCommand(parsed))
					{
						string name = "-------------------- ";
						name.append(*Commands->at(i)->GetName());
						name.append(" --------------------");

						Console->WriteLine(&name);
						Commands->at(i)->Run(parsed);
						Console->WriteLine(&name);
						break;
					}
				}
			}

			FreeUpMemory();
			delete parsed;
		}
		catch (exception* e)
		{
			string *message = new string("!Error!: ");
			message->append(e->what());
			Console->WriteLine(message);
			ToDelete->push_back(message);
			continue;
		}
	}
}

/// <summary>
/// Supposedly frees up all of that memory we were forgetting about.
/// </summary>
void Loop::FreeUpMemory()
{
	ToDelete->swap(*ToDelete);
}

/// <summary>
/// Has some startup logic.
/// </summary>
void Loop::Startup()
{
	Filing* fil = new Filing();
	fil->Startup();
	ToDelete->push_back(fil);

	AddAllCommands();

	size_t length = Commands->size();
	for (size_t i = 0; i < length; i++)
	{
		CommandNames->push_back(Commands->at(i)->GetName());
	}
	std::sort(CommandNames->begin(), CommandNames->end());

	this->FreeUpMemory();
}

/// <summary>
/// Adds all known commands to the commands list.
/// </summary>
void Loop::AddAllCommands()
{
	Commands->push_back(new CommandHelp());
	Commands->push_back(new CommandStart());
}

//
//
//
//
//// This is the callback function. Consider it the event that is raised when, in this case, 
//// a key is pressed.
//LRESULT CALLBACK Loop::HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode >= 0)
//	{
//		// the action is valid: HC_ACTION.
//		if (wParam == WM_KEYDOWN)
//		{
//			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
//			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
//			// a key (non-system) is pressed.
//			if (kbdStruct.vkCode == VK_F1)
//			{
//				// F1 is pressed!
//				MessageBox(NULL, L"F1 is pressed!", L"key pressed", MB_ICONINFORMATION);
//			}
//		}
//	}
//
//	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
//	return CallNextHookEx(_hook, nCode, wParam, lParam);
//}
//
//void Loop::SetHook()
//{
//	// Set the hook and set it to use the callback function above
//	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
//	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
//	// function that sets and releases the hook. If you create a hack you will not need the callback function 
//	// in another place then your own code file anyway. Read more about it at MSDN.
//	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD, HookCallback, NULL, 0)))
//	{
//		MessageBox(NULL, L"Failed to install hook!", L"Error", MB_ICONERROR);
//	}
////}
//
//void Loop::ReleaseHook()
//{
//	UnhookWindowsHookEx(_hook);
//}