#include "stdafx.h"
#include "StepMath.h"

using namespace mth;

StepMath::StepMath()
{
}


StepMath::~StepMath()
{
}

void mth::StepMath::add(Number a, Number b, Number& ret, __int8 & errorCode)
{
	errorCode = this->rangeCheckAdd(a, b);

	if (b.number < 0)
	{
		string step1 = to_string(a.number);
		step1.append(" +");
		step1.append(to_string(b.number));
		step1.append(" = ");
		this->shownSteps->push_back(step1);
		return this->subtract(a, Number(b.number * -1), ret, errorCode);
	}
	else
	{
		string step2 = to_string(a.number);
		step2.append(" + ");
		step2.append(to_string(b.number));
		step2.append(" = ");
		step2.append(to_string(a.number + b.number));
		this->shownSteps->push_back(step2);

	}
	ret = a.number + b.number;
}

void mth::StepMath::subtract(Number a, Number b, Number& ret, __int8& errorCode)
{
	errorCode = this->rangeCheckSubtract(a, b);
	if (b.number == 0)
	{
		string step1 = to_string(a.number);
		step1.append(" - 0 = ");
		step1.append(to_string(a.number));
		this->shownSteps->push_back(step1);
		ret = a.number;
	}

	if (b.number > 0)
	{
		string step2 = to_string(a.number);
		step2.append(" - ");
		step2.append(to_string(b.number));
		step2.append(" = ");
		step2.append(to_string(a.number - b.number));
		this->shownSteps->push_back(step2);
		ret = Number(a.number - b.number);
	}
	else
	{
		string step3 = to_string(a.number);
		step3.append(" -");
		step3.append(to_string(b.number));
		step3.append(" = ");
		this->shownSteps->push_back(step3);
		this->add(a, Number(b.number * -1), ret, errorCode);
	}
}

void mth::StepMath::multiply(Number a, Number b, Number& ret, __int8& errorCode)
{
	errorCode = this->rangeCheckMultiply(a, b);

	string step1 = to_string(a.number);
	step1.append(" * ");
	step1.append(to_string(b.number));
	step1.append(" = ");
	step1.append(to_string(a.number * b.number));
	this->shownSteps->push_back(step1);
	ret =  Number(a.number * b.number);
}

void mth::StepMath::divide(Number a, Number b, Number& ret, __int8& errorCode)
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
	step.append(to_string(a.number / b.number));
	this->shownSteps->push_back(step);
	if (a.number % b.number != 0)
	{
		errorCode = MTH_PRECISION_LOSS;
		ret =  Number(MTH_PRECISION_LOSS);
	}
	ret =  Number(a.number / b.number);
}

__int8 mth::StepMath::rangeCheckAdd(Number a, Number b)
{
	Number c = b;
	c.number * -1;

	return this->rangeCheckSubtract(a, c);
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

void mth::StepMath::publishError(__int8& errorCode)
{
	this->shownSteps->push_back("Error code: " + to_string(errorCode));
}

void mth::IMathObject::toString(string& out)
{
	out = "SOMEONE FORGOT TO DEFINE TOSTRING(). OOPS.";
}
