#pragma once

#include <string>
#include <Windows.h>

using namespace std;

/// <summary>
/// Used to do some utility functions.
/// </summary>
class Util
{
public:

	/// <summary>
	/// Converts a string to a wstring.
	/// </summary>
	/// <param name="Str"></param>
	/// <returns></returns>
	wstring StringToWString(const string& Str);

	/// <summary>
	/// Returns the next line inputted by the console.
	/// </summary>
	/// <returns></returns>
	string* GetLine();

	template<class _Elem,
		class _Traits,
		class _Alloc> inline
		basic_istream<_Elem, _Traits>& getline(
			basic_istream<_Elem, _Traits>&& _Istr,
			basic_string<_Elem, _Traits, _Alloc>& _Str)
	{	// get characters into string, discard newline
		return (getline(_Istr, _Str, _Istr.widen('\n')));
	}

	template<class _Elem,
		class _Traits,
		class _Alloc> inline
		basic_istream<_Elem, _Traits>& getline(
			basic_istream<_Elem, _Traits>& _Istr,
			basic_string<_Elem, _Traits, _Alloc>& _Str)
	{	// get characters into string, discard newline
		return (getline(_STD move(_Istr), _Str, _Istr.widen('\n')));
	}

	template<class _Elem,
		class _Traits,
		class _Alloc> inline
		basic_istream<_Elem, _Traits>& getline(
			basic_istream<_Elem, _Traits>&& _Istr,
			basic_string<_Elem, _Traits, _Alloc>& _Str,
			const _Elem _Delim)
	{	// get characters into string, discard delimiter
		typedef basic_istream<_Elem, _Traits> _Myis;

		ios_base::iostate _State = ios_base::goodbit;
		bool _Changed = false;
		const typename _Myis::sentry _Ok(_Istr, true);

		if (_Ok)
		{	// state okay, extract characters
			_TRY_IO_BEGIN
				_Str.erase();
			const typename _Traits::int_type _Metadelim =
				_Traits::to_int_type(_Delim);
			typename _Traits::int_type _Meta = _Istr.rdbuf()->sgetc();

			for (; ; _Meta = _Istr.rdbuf()->snextc())
				if (_Traits::eq_int_type(_Traits::eof(), _Meta))
				{	// end of file, quit
					_State |= ios_base::eofbit;
					break;
				}
				else if (_Traits::eq_int_type(_Meta, _Metadelim))
				{	// got a delimiter, discard it and quit
					_Changed = true;
					_Istr.rdbuf()->sbumpc();
					break;
				}
				else if (_Str.max_size() <= _Str.size())
				{	// string too large, quit
					_State |= ios_base::failbit;
					break;
				}
				else
				{	// got a character, add it to string
					_Str += _Traits::to_char_type(_Meta);
					_Changed = true;
				}
			_CATCH_IO_(_Istr)
		}

		if (!_Changed)
			_State |= ios_base::failbit;
		_Istr.setstate(_State);
		return (_Istr);
	}

	template<class _Elem,
		class _Traits,
		class _Alloc> inline
		basic_istream<_Elem, _Traits>& getline(
			basic_istream<_Elem, _Traits>& _Istr,
			basic_string<_Elem, _Traits, _Alloc>& _Str,
			const _Elem _Delim)
	{	// get characters into string, discard delimiter
		return (getline(_STD move(_Istr), _Str, _Delim));
	}
};