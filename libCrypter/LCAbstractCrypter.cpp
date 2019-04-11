#include "StdAfx.h"
#include "LCAbstractCrypter.h"
//-----------------------------------------------------------------------------
LCAbstractCrypter::LCAbstractCrypter(CrypterMode mode) :
	Mode(mode),
	ErrorString("No error.")
{

}
//-----------------------------------------------------------------------------
LCAbstractCrypter::~LCAbstractCrypter()
{

}
//-----------------------------------------------------------------------------
CrypterMode LCAbstractCrypter::StringToMode(const char *String)
{
	CrypterMode Result = CrypterMode::Unknown;
	if (String)
	{
		int Size = strlen(String);
		if (Size > 0)
		{
			std::string Temp(String);
			std::transform(Temp.begin(), Temp.end(), Temp.begin(), ::tolower);

			if (Temp == "png")
			{
				Result = CrypterMode::PNG;
			}
		}
	}

	return Result;
}
//-----------------------------------------------------------------------------
std::string LCAbstractCrypter::GetErrorString() const
{
	return ErrorString;
}
//-----------------------------------------------------------------------------
void LCAbstractCrypter::SetErrorString(const std::string &error_string)
{
	ErrorString = error_string;
}
//-----------------------------------------------------------------------------
