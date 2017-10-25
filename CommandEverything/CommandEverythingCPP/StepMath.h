/*
This file is used to do math step by step, and display the results of this step by step.
*/

#pragma once

/// <summary>
/// Represents a fraction.
/// </summary>
struct Fraction
{
	__int64 numerator;
	__int64 denominator;
};

/// <summary>
/// Represents a number in a radical.
/// </summary>
struct Radical
{
	/// <summary>
	/// The N root of number.
	/// </summary>
	__int64 index;

	/// <summary>
	/// The number under the radical.
	/// </summary>
	__int64 number;
};

struct RadicalFraction
{
	Radical numerator;
	Radical denominator;
};

/// <summary>
/// Represents a fraction with a radical as the numerator and a normal number as the denominator.
/// </summary>
struct TRadicalFraction
{
	Radical numerator;
	__int64 denominator;
};

/// <summary>
/// Represents a fraction with a normal number as the numerator and a radical as the deonominator.
/// </summary>
struct BRadicalFraction
{
	__int64 numerator;
	Radical denominator;
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

//Future:
#define MultiplicationProblem 2
#define DivisionProblem 3
#define Exponent 4
#define SolveFor1Variable 5 //Any problem with just 1 variable
#define Sin 6
#define Cos 7
#define Tan 8
#define Csc 9
#define Sec 10
#define Cot 11


#pragma endregion

#pragma region Error_Codes

#define NO_ERROR 0
#define DIVIDE_BY_ZERO 1
#define UNDEFINED 2

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
	__int64 add(__int64 a, __int64 b);

	/// <summary>
	/// Subtracts 'b' from 'a'.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	__int64 subtract(__int64 a, __int64 b);

	/// <summary>
	/// Multiplies 'a' by 'b'.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	__int64 multiply(__int64 a, __int64 b);

	/// <summary>
	/// Divides 'b' from 'a'.
	/// This does not keep precision if a % b != 0.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="errorCode>If a error has occurred, it will be set here.</param>
	/// <returns></returns>
	__int64 divide(__int64 a, __int64 b, __int8& errorCode);
};