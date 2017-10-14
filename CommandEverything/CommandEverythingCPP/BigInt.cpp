#include "stdafx.h"
#include "BigInt.h"


BigInt::BigInt()
{
}

BigInt::BigInt(__int64& num)
{
	unsigned __int8 digitCount = this->count(num);
	unsigned __int8 length = digitCount;

	if (this->digitSize < digitCount)
	{
		realloc(this->digits, digitCount);
	}

	unsigned __int8 i = 0;
	while (digitCount--)
	{
		this->digits[i] = num % 10;
		num /= 10;
		++i;
	}
}

BigInt::BigInt(unsigned __int64& num)
{
	unsigned __int8 digitCount = this->count(num);
	unsigned __int8 length = digitCount;

	if (this->digitSize < digitCount)
	{
		realloc(this->digits, digitCount);
	}

	unsigned __int8 i = 0;
	while (digitCount--)
	{
		this->digits[i] = num % 10;
		num /= 10;
		++i;
	}
}

BigInt::~BigInt()
{
	delete this->digits;
}

BigInt& BigInt::operator+(BigInt& mathObject)
{
	//Gonna add just like I would by hand.

	//The size of the smaller array.
	unsigned __int64 length;

	if (mathObject.digitSize > this->digitSize)
	{
		length = mathObject.digitSize;
		realloc(this->digits, mathObject.digitSize + 1);
		this->digitSize = mathObject.digitSize + 1;
	}
	else
	{
		length = mathObject.digitSize;
	}

	//when adding it is possible to overflow up 1 digit.

	//Prepare the arrays to add.
	for (unsigned __int64 i = 0; i < length; ++i)
	{

			this->digits[i] += mathObject.digits[i];
		
			//Carry the 1...
		if (this->digits[i] > 9)
		{
			this->digits[i + 1] = 1;
			this->digits[i] -= 9;
		}
	}

	unsigned __int64 actualLength;

	//Time to remove the excess digits.
	//First, I have to find out the size of my int array that isn't just useless 0's.
	for (unsigned __int64 i = length; i > 0; --i)
	{
		if (this->digits[i] != 0)
		{
			actualLength = i;
			break;
		}
	}
}

unsigned __int8 BigInt::count(__int64 i)
{
	unsigned __int8 ret = 1;
	while (i /= 10) ret++;
	return ret;
}

unsigned __int8 BigInt::count(unsigned __int64 i)
{
	unsigned __int8 ret = 1;
	while (i /= 10) ret++;
	return ret;
}