#include "stdafx.h"
#include "TabCompletion.h"


TabCompletion::TabCompletion()
{
}


TabCompletion::~TabCompletion()
{
}

string * TabCompletion::Suggest(vector<string*>* PossibleValues, string * InputSoFar)
{
	return nullptr;
}

string * TabCompletion::binary_search(vector<string*>* array, int first, int last, string *search_key)
{
	string* index;

	if (first > last)
	{
		index = NULL;
	}
	else
	{
		int mid = (first + last) / 2;

		if (search_key == array->at(mid))
		{
			index = array->at(mid);
		}
		else
		{
			if (search_key < array->at(mid))
			{
				index = binary_search(array, first, mid - 1, search_key);
			}
			else
			{
				index = binary_search(array, mid + 1, last, search_key);
			}
		}
	}
		return index;
}