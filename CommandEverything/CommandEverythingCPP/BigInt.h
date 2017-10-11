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

	BigInt& operator=(BigInt& mathObject);
	BigInt& operator=(BigInt* mathObject);

	BigInt& operator+(BigInt& mathObject);
	BigInt& operator+(BigInt* mathObject);
	BigInt& operator+=(BigInt& mathObject);
	BigInt& operator+=(BigInt* mathObject);

	BigInt& operator-(BigInt& mathObject);
	BigInt& operator-(BigInt* mathObject);
	BigInt& operator-=(BigInt& mathObject);
	BigInt& operator-=(BigInt* mathObject);

	BigInt& operator*(BigInt& mathObject);
	BigInt& operator*(BigInt* mathObject);
	BigInt& operator*=(BigInt& mathObject);
	BigInt& operator*=(BigInt* mathObject);

	BigInt& operator/(BigInt& mathObject);
	BigInt& operator/(BigInt* mathObject);
	BigInt& operator/=(BigInt& mathObject);
	BigInt& operator/=(BigInt* mathObject);

	BigInt& operator%(BigInt& mathObject);
	BigInt& operator%(BigInt* mathObject);
	BigInt& operator%=(BigInt& mathObject);
	BigInt& operator%=(BigInt* mathObject);

	bool operator==(BigInt& mathObject);
	bool operator==(BigInt* mathObject);

	bool operator!=(BigInt& mathObject);
	bool operator!=(BigInt* mathObject);

	bool operator>(BigInt& mathObject);
	bool operator>(BigInt* mathObject);

	bool operator<(BigInt& mathObject);
	bool operator<(BigInt* mathObject);

	bool operator>=(BigInt& mathObject);
	bool operator>=(BigInt* mathObject);

	bool operator<=(BigInt& mathObject);
	bool operator<=(BigInt* mathObject);

private:
	/// <summary>
	/// An array of digits.
	/// </summary>
	TwoDigit* digits;
};