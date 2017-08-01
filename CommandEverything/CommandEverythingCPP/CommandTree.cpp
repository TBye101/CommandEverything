#include "stdafx.h"
#include "CommandTree.h"


CommandTree::CommandTree()
{
}


CommandTree::~CommandTree()
{
}

bool CommandTree::ShouldRunThisCommand(ParsedCommand* Parsed)
{
	return (Parsed->Words->at(0) == "tree");
}

void CommandTree::Run(ParsedCommand* Parsed)
{
}

string* CommandTree::GetName()
{
	return new string("Tree");
}

string* CommandTree::GetHelp()
{
	return new string("Shows a tree structure just like CMDs. Lets you get an idea of the directory structure while using command line.");
}
