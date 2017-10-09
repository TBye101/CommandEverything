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

	/// <summary>
	/// Subtracts mathObject from this IMathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual IMathObject& operator-(IMathObject& mathObject) = 0;
	virtual IMathObject& operator-(IMathObject* mathObject) = 0;

	/// <summary>
	/// Multiplies this IMathObject by mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual IMathObject& operator*(IMathObject& mathObject) = 0;
	virtual IMathObject& operator*(IMathObject* mathObject) = 0;

	/// <summary>
	/// Divides this IMathObject by mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual IMathObject& operator/(IMathObject& mathObject) = 0;
	virtual IMathObject& operator/(IMathObject* mathObject) = 0;

	/// <summary>
	/// Returns the value of this IMathObject % mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual IMathObject& operator%(IMathObject& mathObject) = 0;
	virtual IMathObject& operator%(IMathObject* mathObject) = 0;

	/// <summary>
	/// Determines if mathObject is equal to this object.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual bool operator==(IMathObject& mathObject) = 0;
	virtual bool operator==(IMathObject* mathObject) = 0;

	/// <summary>
	/// Determines if mathObject does not equal this object.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual bool operator!=(IMathObject& mathObject) = 0;
	virtual bool operator!=(IMathObject* mathObject) = 0;
	
	/// <summary>
	/// Determines if this IMathObject is greater than mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual bool operator>(IMathObject& mathObject) = 0;
	virtual bool operator>(IMathObject* mathObject) = 0;

	/// <summary>
	/// Determines if this IMathObject is less than mathObject.
	/// </summary>
	/// <param name="mathObject"></param>
	/// <returns></returns>
	virtual bool operator<(IMathObject& mathObject) = 0;
	virtual bool operator<(IMathObject* mathObject) = 0;
};