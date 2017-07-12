#pragma once

/// <summary>
/// Does tab completion results.
/// </summary>
class TabCompletion
{
public:
	TabCompletion();
	~TabCompletion();

	/// <summary>
	/// Suggests a tab completion value based on the list of possible answers given, and the input so far.
	/// Inputted list must have more than 1 item to work properly.
	/// </summary>
	/// <param name="PossibleValues">This list must be sorted before passing.</param>
	/// <param name="InputSoFar"></param>
	/// <returns></returns>
	string* Suggest(vector<string*>* PossibleValues, string* InputSoFar);
private:
	/// <summary>
	/// Returns null if an error occurs.
	/// This binary search searches for which string has the most same letters consecutively at the start of the string.
	/// </summary>
	/// <param name="array"></param>
	/// <param name="first"></param>
	/// <param name="last"></param>
	/// <param name="search_key"></param>
	/// <returns></returns>
	string* binary_search(vector<string*>* array, int first, int last, string* search_key);

	/// <summary>
	/// Determines how many characters the strings have in common at the start of the string.
	/// </summary>
	/// <param name="One"></param>
	/// <param name="Two"></param>
	/// <returns></returns>
	__int16 CharactersInCommon(string* One, string* Two);
};