#pragma once

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

#pragma endregion

#pragma region Error_Codes

#define NO_ERROR 0
#define DIVIDE_BY_ZERO 1

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
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="errorCode>If a error has occurred, it will be set here.</param>
	/// <returns></returns>
	__int64 divide(__int64 a, __int64 b, __int8& errorCode);
};