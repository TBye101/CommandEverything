#pragma once

#include <string>
#include <vector>


struct TwoDigit
{
	__int8 digit1 : 4;
	__int8 digit2 : 4;
};

class BigInt
{
public:

	BigInt();
	BigInt(__int64& num);
	BigInt(unsigned __int64& num);

	~BigInt();

	BigInt& operator+(BigInt& mathObject);
	//BigInt& operator+(BigInt* mathObject);
	//BigInt& operator+=(BigInt& mathObject);
	//BigInt& operator+=(BigInt* mathObject);
	BigInt& operator+(__int64& num);
	BigInt& operator+=(__int64& num);

	//BigInt& operator-(BigInt& mathObject);
	//BigInt& operator-(BigInt* mathObject);
	//BigInt& operator-=(BigInt& mathObject);
	//BigInt& operator-=(BigInt* mathObject);

	//BigInt& operator*(BigInt& mathObject);
	//BigInt& operator*(BigInt* mathObject);
	//BigInt& operator*=(BigInt& mathObject);
	//BigInt& operator*=(BigInt* mathObject);

	//BigInt& operator/(BigInt& mathObject);
	//BigInt& operator/(BigInt* mathObject);
	//BigInt& operator/=(BigInt& mathObject);
	//BigInt& operator/=(BigInt* mathObject);

	//BigInt& operator%(BigInt& mathObject);
	//BigInt& operator%(BigInt* mathObject);
	//BigInt& operator%=(BigInt& mathObject);
	//BigInt& operator%=(BigInt* mathObject);

	//bool operator==(BigInt& mathObject);
	//bool operator==(BigInt* mathObject);

	//bool operator!=(BigInt& mathObject);
	//bool operator!=(BigInt* mathObject);

	//bool operator>(BigInt& mathObject);
	//bool operator>(BigInt* mathObject);

	//bool operator<(BigInt& mathObject);
	//bool operator<(BigInt* mathObject);

	//bool operator>=(BigInt& mathObject);
	//bool operator>=(BigInt* mathObject);

	//bool operator<=(BigInt& mathObject);
	//bool operator<=(BigInt* mathObject);

private:

	/// <summary>
	/// All the digits in this number digits.
	/// Order of digits: 9, 8, 7, 6, 5, 4, 3, 2, 1...
	/// </summary>
	__int8* digits = new __int8[1];

	/// <summary>
	/// The size of the digits array.
	/// </summary>
	unsigned __int64 digitSize = 1;

	/// <summary>
	/// Counts the number of digits in a int.
	/// </summary>
	/// <param name="i"></param>
	/// <returns></returns>
	unsigned __int8 count(__int64 i);
	unsigned __int8 count(unsigned __int64 i);
};