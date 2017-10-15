#include "stdafx.h"
#include "BigInt.h"

BigInt::BigInt(__int64 a)
{
	unsigned __int8 digitCount = this->count(a);
	
	unsigned __int8 i = 0;
	__int8* digs = new __int8[digitCount];
	while (i != digitCount)
	{
		digs[i] = this->getDigit(a, i);
		++i;
	}

	this->setDigits(digs, digitCount);
	delete digs;
}

BigInt::~BigInt()
{
}

BigInt& BigInt::operator+(BigInt& mathObject)
{
	//Gonna add just like I would by hand.

	unsigned __int64 length;
	
	if (this->digits->size() > mathObject.digits->size())
	{
		length = this->digits->size();
	}
	else
	{
		length = mathObject.digits->size();
	}

	//Each TwoDigit has two numbers, and when adding it is possible to overflow up 1 digit.

	//The contents of this BigInt
	__int8* newNumber = new __int8[(length * 2) + 1];

	//Two digits in a TwoDigit struct.
	length *= 2;

	//Prepare the arrays to add.
	for (unsigned __int64 i = 0; i < length; ++i)
	{
		//Do the second section of a TwoDigit
		if (i % 2 == 1)
		{
			newNumber[i] += this->digits->at(i / 2).digit2 + mathObject.digits->at(i / 2).digit2;
		}
		//Do the first section of a TwoDigit
		else
		{
			newNumber[i] += this->digits->at(i / 2).digit1 + mathObject.digits->at(i / 2).digit1;
		}

		//Carry the 1...
		if (newNumber[i] > 9)
		{
			newNumber[i + 1] = 1;
			newNumber[i] -= 9;
		}
	}

	unsigned __int64 actualLength;
	//Time to remove the excess digits.
	//First, I have to find out the size of my int array that isn't just useless 0's.
	for (unsigned __int64 i = length; i > 0; --i)
	{
		if (newNumber[i] != 0)
		{
			actualLength = i;
			break;
		}
	}

	this->setDigits(newNumber, actualLength);
	delete newNumber;
	return *this;
}

int* BigInt::getDigits(unsigned __int64 size)
{
	size = this->digits->size();
	int* ret = new int[size * 2];

	unsigned __int64 length = size * 2;
	for (unsigned __int64 i = 0; i < length; i++)
	{
		//If it's time to do the second section of a TwoDigit
		if (i % 2 == 1)
		{
			ret[i] = this->digits->at(i / 2).digit2;
		}
		//Do the first section of a TwoDigit
		else
		{
			ret[i]  = this->digits->at(i / 2).digit1;
		}
	}

	return ret;
}

void BigInt::setDigits(__int8* yourDigits, unsigned __int64 size)
{
	this->digits->clear();
	TwoDigit digitSet;

	for (unsigned __int64 i = 0; i < size; i++)
	{
		//If it's time to set the second section.
		if (i % 2 == 1)
		{
			digitSet.digit2 = yourDigits[i];
			this->digits->push_back(digitSet);
		}
		//If it's time to set the first section.
		else
		{
			digitSet.digit1 = yourDigits[i];
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

__int8 BigInt::getDigit(__int64 num, unsigned __int8 position)
{
	__int8 ret =  (num / (int)pow(10, floor(log10(num)) - position)) % 10;
	return ret;
}