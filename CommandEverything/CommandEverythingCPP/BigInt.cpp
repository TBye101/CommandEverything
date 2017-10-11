#include "stdafx.h"
#include "BigInt.h"


BigInt::BigInt()
{
	this->digits = new TwoDigit[1];
}

BigInt::~BigInt()
{
}