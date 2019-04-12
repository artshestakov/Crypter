#include "StdAfx.h"
#include "LCGlobal.h"
#include "LCCryptPNG.h"
//-----------------------------------------------------------------------------
std::string ErrorString;
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Crypt(const char *PathImage, const char *PathResult, const char *Message)
{
	LCCryptPNG Crypter;
	if (!Crypter.Crypt(PathImage, PathResult, Message))
	{
		ErrorString = Crypter.GetErrorString();
		return R_ERROR;
	}

	return R_OK;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) const char* Decrypt(const char *PathImage)
{
	std::string Message;

	LCCryptPNG Decrypter;
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
