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
	void setDigits(int* yourDigits, unsigned __int64 size);
};