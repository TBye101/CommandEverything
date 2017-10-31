#include "stdafx.h"
#include "StepMath.h"

using namespace mth;

StepMath::StepMath()
{
}


StepMath::~StepMath()
{
}

Number mth::Number::add(Number a, Number b, __int8& errorCode)
{
	errorCode = this->rangeCheckAdd(a, b);

	if (b.number < 0)
	{
		string step1 = to_string(a.number);
		step1.append(" +");
		step1.append(to_string(b.number));
		step1.append(" = ");
		shownSteps->push_back(step1);
		return this->subtract(a, Number(b.number * -1), errorCode);
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

	return Number(number + b.number);
}

Number mth::Number::subtract(Number a, Number b, __int8& errorCode)
{
	Number ret;

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
		ret =  this->add(a, Number(b.number * -1), errorCode);
	}

	return ret;
}

Number mth::Number::multiply(Number a, Number b, __int8& errorCode)
{
	Number ret;
	errorCode = this->rangeCheckMultiply(a, b);

	string step1 = to_string(a.number);
	step1.append(" * ");
	step1.append(to_string(b.number));
	step1.append(" = ");
	step1.append(to_string(a.number * b.number));
	mth::shownSteps->push_back(step1);
	ret =  Number(a.number * b.number);

	return ret;
}

mth::IMathObject mth::Number::divide(Number a, Number b, __int8& errorCode)
{
	//Throw a divide by zero error here.
	if (b.number == 0)
	{
		errorCode = MTH_DIVIDE_BY_ZERO;
		mth::shownSteps->push_back("Divide by Zero: Undefined");
		return Number(0);
	}

	
	string step = to_string(a.number);
	step.append(" / ");
	step.append(to_string(b.number));
	step.append(" = ");

	mth::Fraction r = mth::Fraction(a, b);
	r = mth::StepMath::simplifyFraction(r.numerator, r.denominator, errorCode);

	step.append(r.toString());
	mth::shownSteps->push_back(step);

	return r;
}

bool mth::Number::equals(Number& a, Number& b, __int8& errorCode)
{
	return (a.number == b.number);
}

__int8 mth::Number::rangeCheckAdd(Number a, Number b)
{
	Number c = b;
	c.number *= -1;

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

Fraction mth::StepMath::simplifyFraction(IMathObject& numerator, IMathObject& denominator, __int8 & errorCode)
{
	errorCode = MTH_UNSIMPLIFIABLE;
	return Fraction(numerator, denominator);
}

Fraction mth::StepMath::simplifyFraction(Number& numerator, Number& denominator, __int8& errorCode)
{
	Fraction ret;
	Number gcd = Number(StepMath::greatestCommonDivisor(numerator.number, denominator.number));
	ret.numerator = numerator.divide(numerator, gcd, errorCode);
	ret.denominator = denominator.divide(denominator, gcd, errorCode);
	return ret;
}

void mth::StepMath::publishError(__int8& errorCode)
{
	shownSteps->push_back("Error code: " + to_string(errorCode));
}

__int64 mth::StepMath::greatestCommonDivisor(__int64 a, __int64 b)
{
		return b == 0 ? a : mth::StepMath::greatestCommonDivisor(b, a % b);
}

IMathObject mth::IMathObject::add(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::IMathObject::subtract(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::IMathObject::multiply(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::IMathObject::divide(IMathObject& a, IMathObject& c, __int8& errorCode)
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

IMathObject mth::Fraction::add(Fraction& a, IMathObject& b, __int8& errorCode)
{

}

IMathObject mth::Fraction::add(Fraction& a, Fraction& b, __int8& errorCode)
{
	/*
	Equation used:
	a/b + c/d = (ad + cb)/bd
	*/

	//IMathObject numerator = a.numerator.add(a.numerator.add(a.numerator, b.denominator, errorCode), b.numerator.add(b.numerator, a.denominator, errorCode), errorCode);
	//IMathObject denominator;

	//b.numerator.multiply(a.denominator, b.numerator, numerator, errorCode);
	//a.numerator.multiply(a.numerator, b.denominator, denominator, errorCode);
	

	return Fraction(numerator, denominator);
}
