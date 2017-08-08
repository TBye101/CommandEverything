#pragma once
class CommandDecrypt : public ICommand
{
public:
	CommandDecrypt();
	~CommandDecrypt();

	bool ShouldRunThisCommand(ParsedCommand* Parsed);

	void Run(ParsedCommand* Parsed);

	string* GetName();

	string* GetHelp();
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
	string DecryptChar(string* Msg, const char* Key);

	void Go();
};

