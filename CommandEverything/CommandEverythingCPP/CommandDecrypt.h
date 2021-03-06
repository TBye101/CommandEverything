#pragma once
class CommandDecrypt : public ICommand
{
public:
	CommandDecrypt();
	~CommandDecrypt();

	bool shouldRunThisCommand(ParsedCommand* Parsed);

	void run(ParsedCommand* Parsed);

	string* getName();

	string* getHelp();
private:
	/// <summary>
	/// The key we encrypt off of.
	/// </summary>
	string* EncryptionKey;

	vector<string> Cmd;

	/// <summary>
	/// Encrypts a string.
	/// </summary>
	/// <param name="Msg"></param>
	/// <param name="Key"></param>
	/// <returns></returns>
	void DecryptChar(vector<__int16>* Msg, const char* Key, vector<__int16>* storage);

	void Go();
};
