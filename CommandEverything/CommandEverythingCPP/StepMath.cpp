#include "stdafx.h"
#include "StepMath.h"

using namespace mth;

StepMath::StepMath()
{
}


StepMath::~StepMath()
{
}

void mth::Number::add(Number a, Number b, Number& ret, __int8& errorCode)
{
	errorCode = this->rangeCheckAdd(a, b);

	if (b.number < 0)
	{
		string step1 = to_string(a.number);
		step1.append(" +");
		step1.append(to_string(b.number));
		step1.append(" = ");
		shownSteps->push_back(step1);
		return this->subtract(a, Number(b.number * -1), ret, errorCode);
	}
	else
	{
		string step2 = to_string(a.number);
		step2.append(" + ");
		step2.append(to_string(b.number));
		step2.append(" = ");
		step2.append(to_string(a.number + b.number));
		shownSteps->push_back(step2);

	}
	ret = a.number + b.number;
}

void mth::Number::subtract(Number a, Number b, Number& ret, __int8& errorCode)
{
	errorCode = this->rangeCheckSubtract(a, b);
	if (b.number == 0)
	{
		string step1 = to_string(a.number);
		step1.append(" - 0 = ");
		step1.append(to_string(a.number));
		shownSteps->push_back(step1);
		ret = a.number;
	}

	if (b.number > 0)
	{
		string step2 = to_string(a.number);
		step2.append(" - ");
		step2.append(to_string(b.number));
		step2.append(" = ");
		step2.append(to_string(a.number - b.number));
		shownSteps->push_back(step2);
		ret = Number(a.number - b.number);
	}
	else
	{
		string step3 = to_string(a.number);
		step3.append(" -");
		step3.append(to_string(b.number));
		step3.append(" = ");
		shownSteps->push_back(step3);
		this->add(a, Number(b.number * -1), ret, errorCode);
	}
}

void mth::Number::multiply(Number a, Number b, Number& ret, __int8& errorCode)
{
	errorCode = this->rangeCheckMultiply(a, b);

	string step1 = to_string(a.number);
	step1.append(" * ");
	step1.append(to_string(b.number));
	step1.append(" = ");
	step1.append(to_string(a.number * b.number));
	shownSteps->push_back(step1);
	ret =  Number(a.number * b.number);
}

void mth::Number::divide(Number a, Number b, IMathObject& ret, __int8& errorCode)
{
	//Throw a divide by zero error here.
	if (b.number == 0)
	{
		errorCode = MTH_DIVIDE_BY_ZERO;
		//this->shownSteps->push_back("Divide by Zero: Undefined");
		ret = Number(0);
	}

	
	string step = to_string(a.number);
	step.append(" / ");
	step.append(to_string(b.number));
	step.append(" = ");

	Fraction r = Fraction(a, b);
	mth::StepMath::simplifyFraction(r.numerator, r.denominator, r, errorCode);

	step.append(r.toString());
	ret = r;
	shownSteps->push_back(step);
}

bool mth::Number::equals(Number& a, Number& b, __int8& errorCode)
{
	return (a.number == b.number);
}

__int8 mth::Number::rangeCheckAdd(Number a, Number b)
{
	Number c = b;
	c.number * -1;

	return this->rangeCheckSubtract(a, c);
}

__int8 mth::Number::rangeCheckSubtract(Number a, Number b)
{
	//Do a check to see if this operation will stay within __int64 bounds.
	if (b.number < 0)
	{
		if (INT64_MAX - abs(b.number) < a.number)
		{
			//We will go past the upper bound of our int range here.
			return MTH_OUT_OF_RANGE;
		}
	}
	else
	{
		if ((INT64_MIN + b.number) > a.number)
		{
			//We will drop past the lower range of an int here.
			return MTH_OUT_OF_RANGE;
		}
	}

	return MTH_NO_ERROR;
}

__int8 mth::Number::rangeCheckMultiply(Number a, Number b)
{
	//Check to make sure we won't go past an integer's max or min range.
	if (INT64_MAX / abs(b.number) <= abs(a.number))
	{
		//Operation would have been out of range.
		return MTH_OUT_OF_RANGE;
	}

	return MTH_NO_ERROR;
}

void mth::StepMath::simplifyFraction(IMathObject& numerator, IMathObject& denominator, Fraction& ret, __int8 & errorCode)
{
	errorCode = MTH_UNSIMPLIFIABLE;
}

void mth::StepMath::simplifyFraction(Number& numerator, Number& denominator, Fraction& ret, __int8& errorCode)
{
	Number gcd = Number(StepMath::greatestCommonDivisor(numerator.number, denominator.number));
	numerator.divide(numerator, gcd, ret.numerator, errorCode);
	denominator.divide(denominator, gcd, ret.denominator, errorCode);
}

void mth::StepMath::publishError(__int8& errorCode)
{
	shownSteps->push_back("Error code: " + to_string(errorCode));
}

__int64 mth::StepMath::greatestCommonDivisor(__int64 a, __int64 b)
{
		return b == 0 ? a : mth::StepMath::greatestCommonDivisor(b, a % b);
}

void mth::IMathObject::add(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

void mth::IMathObject::subtract(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

void mth::IMathObject::multiply(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

void mth::IMathObject::divide(IMathObject& a, IMathObject& c, IMathObject& ret, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

bool mth::IMathObject::equals(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

string mth::IMathObject::toString()
{
	return FUNCTION_NOT_IMPLEMENTED;
}

void mth::Fraction::add(Fraction& a, IMathObject& b, IMathObject& ret, __int8& errorCode)
{

}

void mth::Fraction::add(Fraction& a, Fraction& b, IMathObject& ret, __int8& errorCode)
{
	/*
	Equation used:
	a/b + c/d = (ad + cb)/bd
	*/

	IMathObject numerator;
	IMathObject denominator;

	//b.numerator.multiply(a.denominator, b.numerator, numerator, errorCode);
	//a.numerator.multiply(a.numerator, b.denominator, denominator, errorCode);
	

	ret = Fraction(numerator, denominator);
}
