#pragma once

#include "IMathObject.h"

class BigInt : public IMathObject
{
public:

	vector<string>* steps;

	BigInt(vector<string>* step);
	~BigInt();

	IMathObject& operator=(IMathObject& mathObject);
	IMathObject& operator=(IMathObject* mathObject);

	IMathObject& operator+(IMathObject& mathObject);
	IMathObject& operator+(IMathObject* mathObject);
	IMathObject& operator+=(IMathObject& mathObject);
	IMathObject& operator+=(IMathObject* mathObject);

	IMathObject& operator-(IMathObject& mathObject);
	IMathObject& operator-(IMathObject* mathObject);
	IMathObject& operator-=(IMathObject& mathObject);
	IMathObject& operator-=(IMathObject* mathObject);

	IMathObject& operator*(IMathObject& mathObject);
	IMathObject& operator*(IMathObject* mathObject);
	IMathObject& operator*=(IMathObject& mathObject);
	IMathObject& operator*=(IMathObject* mathObject);

	IMathObject& operator/(IMathObject& mathObject);
	IMathObject& operator/(IMathObject* mathObject);
	IMathObject& operator/=(IMathObject& mathObject);
	IMathObject& operator/=(IMathObject* mathObject);

	IMathObject& operator%(IMathObject& mathObject);
	IMathObject& operator%(IMathObject* mathObject);
	IMathObject& operator%=(IMathObject& mathObject);
	IMathObject& operator%=(IMathObject* mathObject);

	bool operator==(IMathObject& mathObject);
	bool operator==(IMathObject* mathObject);

	bool operator!=(IMathObject& mathObject);
	bool operator!=(IMathObject* mathObject);

	bool operator>(IMathObject& mathObject);
	bool operator>(IMathObject* mathObject);

	bool operator<(IMathObject& mathObject);
	bool operator<(IMathObject* mathObject);

	bool operator>=(IMathObject& mathObject);
	bool operator>=(IMathObject* mathObject);

	bool operator<=(IMathObject& mathObject);
	bool operator<=(IMathObject* mathObject);
};