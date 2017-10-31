#include "stdafx.h"
#include "StepMath.h"

using namespace mth;

StepMath::StepMath()
{
}


StepMath::~StepMath()
{
}

Number mth::StepMath::add(Number a, Number b, __int8& errorCode)
{
	errorCode = StepMath::rangeCheckAdd(a, b);

	if (b.number < 0)
	{
		string step1 = to_string(a.number);
		step1.append(" +");
		step1.append(to_string(b.number));
		step1.append(" = ");
		shownSteps->push_back(step1);
		return StepMath::subtract(a, Number(b.number * -1), errorCode);
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

	return StepMath::add(a, b, errorCode);
}

Number mth::StepMath::subtract(Number a, Number b, __int8& errorCode)
{
	Number ret;

	errorCode = StepMath::rangeCheckSubtract(a, b);
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
		ret =  StepMath::add(a, Number(b.number * -1), errorCode);
	}

	return ret;
}

Number mth::StepMath::multiply(Number a, Number b, __int8& errorCode)
{
	Number ret;
	errorCode = StepMath::rangeCheckMultiply(a, b);

	string step1 = to_string(a.number);
	step1.append(" * ");
	step1.append(to_string(b.number));
	step1.append(" = ");
	step1.append(to_string(a.number * b.number));
	mth::shownSteps->push_back(step1);
	ret =  Number(a.number * b.number);

	return ret;
}

mth::IMathObject mth::StepMath::divide(Number a, Number b, __int8& errorCode)
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

__int8 mth::StepMath::rangeCheckAdd(Number a, Number b)
{
	Number c = b;
	c.number *= -1;

	return StepMath::rangeCheckSubtract(a, c);
}

__int8 mth::StepMath::rangeCheckSubtract(Number a, Number b)
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

__int8 mth::StepMath::rangeCheckMultiply(Number a, Number b)
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
	ret.numerator = StepMath::divide(numerator, gcd, errorCode);
	ret.denominator = StepMath::divide(denominator, gcd, errorCode);
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

IMathObject mth::StepMath::add(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::StepMath::subtract(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::StepMath::multiply(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

IMathObject mth::StepMath::divide(IMathObject& a, IMathObject& c, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

bool mth::StepMath::equals(IMathObject& a, IMathObject& b, __int8& errorCode)
{
	throw new exception(FUNCTION_NOT_IMPLEMENTED);
}

string mth::IMathObject::toString()
{
	return FUNCTION_NOT_IMPLEMENTED;
}

IMathObject mth::StepMath::add(Fraction& a, IMathObject& b, __int8& errorCode)
{
	Fraction newB = Fraction(b, Number(1));
	return StepMath::add(a, newB, errorCode);
}

IMathObject mth::StepMath::add(Fraction& a, Fraction& b, __int8& errorCode)
{
	/*
	Equation used:
	a / b + c / d = (ad + cb) / bd
	*/

	IMathObject numerator = StepMath::add(StepMath::multiply(a.numerator, b.denominator, errorCode), StepMath::multiply(b.numerator, a.denominator, errorCode), errorCode);
	IMathObject denominator = StepMath::multiply(a.denominator, b.denominator, errorCode);

	Fraction ret = StepMath::simplifyFraction(numerator, denominator, errorCode);

	return ret;
}
