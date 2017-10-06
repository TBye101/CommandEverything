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
		return this->subtract(a, b);
	}
	else
	{
		string step = to_string(a);
		step.append(" + ");
		step.append(to_string(b));
		step.append(" = ");
		step.append(to_string(a + b));
		this->shownSteps->push_back(step);

	}
	return a + b;
}

__int64 StepMath::subtract(__int64 a, __int64 b)
{
	if (b >= 0)
	{
		string step = to_string(a);
		step.append(" - ");
		step.append(to_string(b));
		step.append(" = ");
		step.append(to_string(a - b));
		this->shownSteps->push_back(step);
		return a - b;
	}
	else
	{
		return this->add(a, b * -1);
	}
}
