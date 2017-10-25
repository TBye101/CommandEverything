/*
This file is used to do math step by step, and display the results of this step by step.
*/

#pragma once

namespace mth
{
	class IMathObject
	{
	public:
		/// <summary>
		/// Converts the object to a string.
		/// </summary>
		/// <param name="out"></param>
		virtual void toString(string& out);
	};

	class Number : IMathObject
	{
	public:
		__int64 number;
		Number(__int64 a)
		{
			number = a;
		}
	};

	/// <summary>
	/// Represents a fraction.
	/// </summary>
	class Fraction : IMathObject
	{
		IMathObject numerator;
		IMathObject denominator;
	};

	/// <summary>
	/// Represents a number in a radical.
	/// </summary>
	class Radical : IMathObject
	{
		/// <summary>
		/// The N root of number.
		/// </summary>
		IMathObject index;

		/// <summary>
		/// The number under the radical.
		/// </summary>
		IMathObject number;
	};

	/// <summary>
	/// Encompasses a math object, and declares to what Nth power it is being raised.
	/// </summary>
	class Exponent : IMathObject
	{
		/// <summary>
		/// The math object that is being raised to the Nth power.
		/// </summary>
		IMathObject exponentSubject;

		IMathObject N;
	};

	/// <summary>
	/// Represents a unknown variable.
	/// </summary>
	class Variable : IMathObject
	{
		/// <summary>
		/// The letter/character that the variable is represented by.
		/// </summary>
		char Symbol;
	};

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
	#define Exponent 4
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

	#pragma endregion

		StepMath();
		~StepMath();

	public:

		/// <summary>
		/// The steps to display to the user on how to solve the math problem.
		/// </summary>
		vector<string>* shownSteps = new vector<string>();

		/// <summary>
		/// Adds 'b' to 'a'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		Number add(Number a, Number b);

		/// <summary>
		/// Subtracts 'b' from 'a'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		Number subtract(Number a, Number b);

		/// <summary>
		/// Multiplies 'a' by 'b'.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		Number multiply(Number a, Number b);

		/// <summary>
		/// Divides 'b' from 'a'.
		/// This does not keep precision if a % b != 0.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="errorCode>If a error has occurred, it will be set here.</param>
		/// <returns></returns>
		Number divide(Number a, Number b, __int8& errorCode);
	};
}