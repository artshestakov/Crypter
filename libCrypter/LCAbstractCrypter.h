#pragma once
//-----------------------------------------------------------------------------
enum CrypterMode
{
	Unknown,
	PNG,
};
//-----------------------------------------------------------------------------
class LCAbstractCrypter
{
public:
	LCAbstractCrypter(CrypterMode mode);
	~LCAbstractCrypter();

	static CrypterMode StringToMode(const char *String);

	virtual bool Crypt(const std::string &PathImage, const std::string &PathOutput, const std::string &Message) = 0;
	virtual bool Decrypt(const std::string &PathImage, std::string &SecretMessage) = 0;
	std::string GetErrorString() const;

protected:
	void SetErrorString(const std::string &error_string);

private:
	CrypterMode Mode;
	std::string ErrorString; //Строка с описанием ошибки
};
//-----------------------------------------------------------------------------
