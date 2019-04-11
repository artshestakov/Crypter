#include "StdAfx.h"
#include "LCAbstractCrypter.h"
#include "LCGlobal.h"
#include "LCCryptPNG.h"
//-----------------------------------------------------------------------------
const char *ErrorString;
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Crypt(const char *Type, const char *PathImage, const char *PathResult, const char *Message)
{
	LCAbstractCrypter *Crypter = CreateCrypter(Type);
	if (!Crypter)
	{
		return R_ERROR;
	}

	if (!Crypter->Crypt(PathImage, PathResult, Message))
	{
		ErrorString = Crypter->GetErrorString().c_str();
		return R_ERROR;
	}

	return R_OK;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Decrypt(const char *PathImage)
{
	return R_OK;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) const char* GetErrorString()
{
	return ErrorString;
}
//-----------------------------------------------------------------------------
LCAbstractCrypter* CreateCrypter(const char *Type)
{
	LCAbstractCrypter *Crypter = nullptr;
	switch (LCAbstractCrypter::StringToMode((char*)Type))
	{
	case PNG: Crypter = new LCCryptPNG(); break;
	}
	return Crypter;
}
//-----------------------------------------------------------------------------
