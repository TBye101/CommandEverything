#pragma once

/// <summary>
/// A interface implemented by variables, numbers, numbers in radicals, imaginary numbers, and other things.
/// </summary>
class IMathObject
{
public:

	/// <summary>
	/// Converts whatever this is to string.
	/// </summary>
	/// <returns></returns>
	virtual string* toString() = 0;

	/// <summary>
	/// Sets this IMathObject equal to mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual IMathObject& operator=(IMathObject& mathObject) = 0;
	virtual IMathObject& operator=(IMathObject* mathObject) = 0;

	/// <summary>
	/// Adds the mathObject to this IMathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	virtual IMathObject& operator+(IMathObject& mathObject) = 0;
	virtual IMathObject& operator+(IMathObject* mathObject) = 0;


};