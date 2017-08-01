// stdafx.cpp : source file that includes just the standard includes
// CommandEverythingCPP.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

template<typename T>
vector<T*>* ToDelete = new vector<T*>();
Writer* Console = new Writer();
Util* Utility = new Util();
Filing* Files = new Filing();
//TabCompletion* Complete = new TabCompletion();

vector<ICommand*>* Commands = new vector<ICommand*>();
vector<string*>* CommandNames = new vector<string*>();
string* FilePath = new string("");

/// <summary>
/// The last user input.
/// </summary>
string Input;