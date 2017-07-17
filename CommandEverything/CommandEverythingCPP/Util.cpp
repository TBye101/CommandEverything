#include "stdafx.h"
#include "Util.h"

wstring Util::StringToWString(const string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

string* Util::GetLine()
{
	string* ret = new string("");
//	string suggestion = "";
//	char last[2];
//	char a;
//
//	//The number of characters we suggested that the user didn't.
//	register unsigned int suggestedchars = 0;
//
//	while (last[1] != '\\' && last[0] != 'r')
//	{
//#if UNICODE
//		a = _getwch();
//#else
//		a = _getch();
//#endif
//		last[0] = last[1];
//		last[1] = a;
//
//		if (a == '\b')
//		{
//			for (register size_t i = 0; i < suggestedchars; i++)
//			{
//				cout << "\b \b";
//				ret->pop_back();
//			}
//		}
//
//		if (a == '\t')
//		{
//			suggestion = *Complete->Suggest(CommandNames, ret);
//			suggestedchars = suggestion.size() - ret->size();
//			cout << suggestion;
//		}
//		else
//		{
//			ret->append(&a);
//		}
//	}

	return ret;
}