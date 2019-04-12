#include "StdAfx.h"
#include "LCGlobal.h"
#include "LCCrypter.h"
//-----------------------------------------------------------------------------
std::string ErrorString;
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Crypt(const char *PathImage, const char *PathResult, const char *Message)
{
	LCCrypter Crypter;
	if (!Crypter.Crypt(PathImage, PathResult, Message))
	{
		ErrorString = Crypter.GetErrorString();
		return 1;
	}

	return 0;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) const char* Decrypt(const char *PathImage)
{
	std::string Message;

	LCCrypter Decrypter;
	if (Decrypter.Decrypt(PathImage, Message))
	{
		char *Result = (char*)malloc(Message.size());
		strcpy(Result, Message.c_str());
		return Result;
	}
	else
	{
		ErrorString = Decrypter.GetErrorString();
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) const char* GetErrorString()
{
	return ErrorString.c_str();
}
//-----------------------------------------------------------------------------
