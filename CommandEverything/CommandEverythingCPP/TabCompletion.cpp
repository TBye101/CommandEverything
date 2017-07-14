#include "stdafx.h"
#include "TabCompletion.h"

TabCompletion::TabCompletion()
{
}

TabCompletion::~TabCompletion()
{
}

string* TabCompletion::Suggest(vector<string*>* PossibleValues, string* InputSoFar)
{
	return this->binary_search(PossibleValues, 0, PossibleValues->size(), InputSoFar);
}

string* TabCompletion::binary_search(vector<string*>* array, int first, int last, string *search_key)
{
	string* index;
	unsigned __int16 ClosestMatch = 0;
	size_t arraylength = array->size();
	unsigned __int16 Zero = this->CharactersInCommon(search_key, array->at(0));
	unsigned __int16 End = this->CharactersInCommon(search_key, array->at(array->size()));

	if (first > last)
	{
		index = NULL;
	}
	else
	{
		int mid = (first + last) / 2;

		if (search_key == array->at(mid) || first == array->size() || last == 0)
		{
			index = array->at(mid);
		}
		else
		{
			if (Zero > End)
			{
				if (this->CharactersInCommon(array->at(mid), search_key) >= ClosestMatch)
				{
					index = array->at(mid);
					ClosestMatch = this->CharactersInCommon(array->at(mid), search_key);
				}

				index = binary_search(array, first, mid - 1, search_key);
			}
			else
			{
				if (this->CharactersInCommon(array->at(mid), search_key) >= ClosestMatch)
				{
					index = array->at(mid);
					ClosestMatch = this->CharactersInCommon(array->at(mid), search_key);
				}

				index = binary_search(array, mid + 1, last, search_key);
			}
		}
	}
	return index;
}

__int16 TabCompletion::CharactersInCommon(string* One, string* Two)
{
	__int16 InCommon = 0;
	size_t length;

	if (One->size() > Two->size())
	{
		length = Two->size();
	}
	else
	{
		length = One->size();
	}

	for (size_t i = 0; i < length; i++)
	{
		if (One[i] == Two[i])
		{
			++InCommon;
		}
		else
		{
			break;
		}
	}

	return InCommon;
}