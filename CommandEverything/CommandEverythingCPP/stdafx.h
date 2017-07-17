// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include "Util.h"
#include <vector>
#include "Writer.h"
#include "Util.h"
#include "ICommand.h"
#include "Filing.h"
//#include "TabCompletion.h"

extern vector<void*>* ToDelete;
extern Writer* Console;
extern Util* Utility;
extern Filing* Files;
//extern TabCompletion* Complete;

/// <summary>
/// All known commands.
/// </summary>
extern vector<ICommand*>* Commands;

/// <summary>
/// Used by the auto complete function.
/// </summary>
extern vector<string*>* CommandNames;


/// <summary>
/// The filepath that this program is currently operating in.
/// Note: Not the filepath to the exe, but rather where the user is accessing files/directories from.
/// In CMD terms, it would be wherever cd is.
/// </summary>
extern string* FilePath;