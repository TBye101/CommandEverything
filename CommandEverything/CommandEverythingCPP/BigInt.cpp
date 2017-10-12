#include "stdafx.h"
#include "BigInt.h"


BigInt::BigInt()
{
}

BigInt::~BigInt()
{
}

BigInt& BigInt::operator+(BigInt& mathObject)
{
	//Gonna add just like I would by hand.

	__int8* topArray = new __int8[this->digits->size() + 1];
	__int8* bottomArray = new __int8[mathObject.digits->size() + 1];
	
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

void BigInt::setDigits(int* yourDigits, unsigned __int64 size)
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