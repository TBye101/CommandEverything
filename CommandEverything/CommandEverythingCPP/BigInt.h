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

	BigInt(__int64 a);
	~BigInt();

	BigInt& operator+(BigInt& mathObject);
	//BigInt& operator+(BigInt* mathObject);
	//BigInt& operator+=(BigInt& mathObject);
	//BigInt& operator+=(BigInt* mathObject);

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
	/// Order of digits: [10 & 9] [8 & 7] [6 & 5]...
	/// </summary>
	vector<TwoDigit>* digits = new vector<TwoDigit>();

	/// <summary>
	/// Converts digits to a int pointer.
	/// </summary>
	/// <param name="size">How many elements are in the array.</param>
	/// <returns></returns>
	int* getDigits(unsigned __int64 size);
	void setDigits(__int8* yourDigits, unsigned __int64 size);

	/// <summary>
	/// Returns how many digits in a number.
	/// </summary>
	/// <param name="i"></param>
	/// <returns></returns>
	unsigned __int8 count(__int64 i);
	unsigned __int8 count(unsigned __int64 i);

	/// <summary>
	/// Gets a specific digit of a number.
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	__int8 getDigit(__int64 num, unsigned __int8 position);
};