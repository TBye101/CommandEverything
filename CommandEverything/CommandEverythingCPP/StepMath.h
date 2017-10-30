/*
# Step Math

## Description

This file is used to do math step by step, and display the results of this step by step.
This library is used for precision math. Decimals or any other form of non-precise answer are not allowed.

## Technical information

All usages of __int8& errorCode are used to return an error code if something goes wrong.
Any parameter values called "ret" are values returned by reference.

*/

#pragma once

namespace mth
{

	/// <summary>
	/// The steps to display to the user on how to solve the math problem.
	/// </summary>
	static vector<string>* shownSteps = new vector<string>();
#define FUNCTION_NOT_IMPLEMENTED "Someone forgot to define this!"

	/// <summary>
	/// The class the holds a bunch of IMathObjects that modify each other. 
	/// </summary>
	//class MathPart
	//{
	//	/// <summary>
	//	/// Holds all of the IMathObjects that modify each other.
	//	/// </summary>
	//	vector<IMathObject> objects;
	//};

	/// <summary>
	/// The base class for various data representations such as a number, fraction, or exponent.
	/// </summary>
	class IMathObject
	{
	public:

		//These functions will throw a exception if you do not define them.
		virtual void add(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode);
		virtual void subtract(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode);
		virtual void multiply(IMathObject& a, IMathObject& b, IMathObject& ret, __int8& errorCode);
		virtual void divide(IMathObject& a, IMathObject& c, IMathObject& ret, __int8& errorCode);

		/// <summary>
		/// Converts the object to a string.
		/// </summary>
		/// <param name="out"></param>
		virtual string toString();
	};

	class Number : public IMathObject
	{
	public:
		__int64 number;
		Number(__int64 a)
		{
			number = a;
		}

		/// <summary>
		/// Adds 'b' to 'a'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		void add(Number a, Number b, Number& ret, __int8& errorCode);

		/// <summary>
		/// Checks to make sure that the addition operation will not cause the integer to fall out of an integer's range.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		__int8 rangeCheckAdd(Number a, Number b);

		/// <summary>
		/// Checks to make sure that the subtraction operation will not cause the integer to fall out of an integer's range.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		__int8 rangeCheckSubtract(Number a, Number b);

		/// <summary>
		/// Checks to make sure that the multiplication operation will not cause the integer to fall out of an integer's range.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		__int8 rangeCheckMultiply(Number a, Number b);

		/// <summary>
		/// Subtracts 'b' from 'a'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		void subtract(Number a, Number b, Number& ret, __int8& errorCode);

		/// <summary>
		/// Multiplies 'a' by 'b'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		void multiply(Number a, Number b, Number& ret, __int8& errorCode);

		/// <summary>
		/// Divides 'b' from 'a'.
		/// This does not keep precision if a % b != 0.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		void divide(Number a, Number b, IMathObject& ret, __int8& errorCode);

		/// <summary>
		///// Divides 'b' from 'a'. This will maintain precision.
		///// </summary>
		///// <param name="a"></param>
		///// <param name="b"></param>
		///// <param name="ret"></param>
		///// <param name="errorCode"></param>
		//void divide(Number a, Number b, Fraction& ret, __int8& errorCode);
	};

	/// <summary>
	/// Represents a fraction.
	/// </summary>
	class Fraction : public IMathObject
	{
	public:

		/// <summary>
		/// The value on top of the fraction.
		/// </summary>
		IMathObject numerator;

		/// <summary>
		/// The value on the bottom of the fraction.
		/// </summary>
		IMathObject denominator;


		Fraction(IMathObject numerator, IMathObject denominator)
		{
			this->numerator = numerator;
			this->denominator = denominator;
		}

		/// <summary>
		/// Simplifies the specified fraction as much as possible.
		/// Currently only supports when the fraction has two integers as it's numerator and denominator.
		/// </summary>
		/// <param name="fraction"></param>
		void simplifyFraction(Fraction& fraction, __int8& errorCode);

		//void add(Fraction& a, IMathObject& b, IMathObject& ret);
		//void add(Fraction& a, Fraction& b, IMathObject& ret);

		//void subtract(IMathObject& a, IMathObject& b, IMathObject& ret);
		//void multiply(IMathObject& a, IMathObject& b, IMathObject& ret);
		//void divide(IMathObject& a, IMathObject& c, IMathObject& ret);
	};

	///// <summary>
	///// Represents a number in a radical.
	///// </summary>
	//class Radical : public IMathObject
	//{
	//	/// <summary>
	//	/// The N root of number.
	//	/// </summary>
	//	IMathObject index;

	//	/// <summary>
	//	/// The number under the radical.
	//	/// </summary>
	//	IMathObject number;
	//};

	///// <summary>
	///// Encompasses a math object, and declares to what Nth power it is being raised.
	///// </summary>
	//class Exponent : public IMathObject
	//{
	//	/// <summary>
	//	/// The math object that is being raised to the Nth power.
	//	/// </summary>
	//	IMathObject exponentSubject;
	//	IMathObject N;
	//};

	/// <summary>
	/// Represents a unknown variable.
	/// </summary>
	//class Variable : public IMathObject
	//{
	//	/// <summary>
	//	/// The letter/character that the variable is represented by.
	//	/// </summary>
	//	char Symbol;
	//};

	/// <summary>
	/// This class is used to do math step by step, and show those steps. 
	/// All answers will be given in simplest form without using decimals if possible.
	/// </summary>
	class StepMath
	{
	public:

	/// <summary>
	/// The type of problems we will support or do support.
	/// </summary>
	#pragma region Problems

	//Supported:

	#define AdditionProblem 0
	#define SubtractionProblem 1
	#define MultiplicationProblem 2
	#define DivisionProblem 3

	//Future:
	#define Xponent 4
	#define SolveFor1Variable 5 //Any problem with just 1 variable
	#define Sin 6
	#define Cos 7
	#define Tan 8
	#define Csc 9
	#define Sec 10
	#define Cot 11
	#define ArcSin 12
	#define ArcCos 13
	#define ArcTan 14
	#define ArcCsc 15
	#define ArcSec 16
	#define ArcCot 17
	#define SqrRoot 18
	#define Log 19

	#pragma endregion

	#pragma region Error_Codes

	#define MTH_NO_ERROR 0
	#define MTH_DIVIDE_BY_ZERO 1
	#define MTH_UNDEFINED 2
	#define MTH_OUT_OF_RANGE 3
	#define MTH_PRECISION_LOSS 4
	#define MTH_INVALID_PARAMETER 5

	#pragma endregion

		StepMath();
		~StepMath();

	public:

#pragma region Utility

		/// <summary>
		/// Pushes the error code and message into the steps stack.
		/// </summary>
		/// <param name="errorCode"></param>
		void publishError(__int8& errorCode);

		/// <summary>
		/// Returns the greatest common divisor (aka greatest common factor) of the two numbers. 
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		__int64 greatestCommonDivisor(__int64 a, __int64 b);

#pragma endregion

	};
}