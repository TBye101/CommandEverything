// stdafx.cpp : source file that includes just the standard includes
// CommandEverythingCPP.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

vector<void*>* ToDelete = new vector<void*>();
Writer* Console = new Writer();
Util* Utility = new Util();
Filing* Files = new Filing();

vector<ICommand*>* Commands = new vector<ICommand*>();