#include "stdafx.h"
#include "StepMath.h"

StepMath::StepMath()
{
}


StepMath::~StepMath()
{
}

__int64 StepMath::add(__int64 a, __int64 b)
{
	if (b < 0)
	{
		string step1 = to_string(a);
		step1.append(" +");
		step1.append(to_string(b));
		step1.append(" = ");
		this->shownSteps->push_back(step1);
		return this->subtract(a, b * -1);
	}
	else 
	{
		string step2 = to_string(a);
		step2.append(" + ");
		step2.append(to_string(b));
		step2.append(" = ");
		step2.append(to_string(a + b));
		this->shownSteps->push_back(step2);

	}
	return a + b;
}

__int64 StepMath::subtract(__int64 a, __int64 b)
{
	if (b == 0)
	{
		string step1 = to_string(a);
		step1.append(" - 0 = ");
		step1.append(to_string(a));
		this->shownSteps->push_back(step1);
		return a;
	}

	if (b > 0)
	{
		string step2 = to_string(a);
		step2.append(" - ");
		step2.append(to_string(b));
		step2.append(" = ");
		step2.append(to_string(a - b));
		this->shownSteps->push_back(step2);
		return a - b;
	}
	else
	{
		string step3 = to_string(a);
		step3.append(" -");
		step3.append(to_string(b));
		step3.append(" = ");
		this->shownSteps->push_back(step3);
		return this->add(a, b * -1);
	}
}

__int64 StepMath::multiply(__int64 a, __int64 b)
{
	//INT64_MAX
	//INT64_MIN
	string step1 = to_string(a);
	step1.append(" * ");
	step1.append(to_string(b));
	step1.append(" = ");
	step1.append(to_string(a * b));
	this->shownSteps->push_back(step1);
	return a * b;
}