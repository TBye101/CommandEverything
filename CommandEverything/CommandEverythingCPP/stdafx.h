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

extern vector<void*>* ToDelete;
extern Writer* Console;
extern Util* Utility;
extern Filing* Files;

/// <summary>
/// All known commands.
/// </summary>
extern vector<ICommand*>* Commands;