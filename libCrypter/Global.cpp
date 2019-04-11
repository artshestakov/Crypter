#include "StdAfx.h"
#include "Global.h"
#include "LCVector.h"
#include "lodepng.h"
#include "LCAbstractCrypter.h"
#include "LCCryptPNG.h"
//-----------------------------------------------------------------------------
const char *ErrorString;
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Crypt(const char *Type, const char *PathImage, const char *PathResult, const char *Message)
{
	LCAbstractCrypter *Crypter = nullptr;
	switch (LCAbstractCrypter::StringToMode((char*)Type))
	{
	case PNG: Crypter = new LCCryptPNG(); break;
	}

	if (!Crypter)
	{
		//ErrorString = std::string("Invalid type: " + std::string(Type)).c_str();
		return R_ERROR;
	}

	bool Result = Crypter->Crypt(PathImage, PathResult, Message);
	return R_OK;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Decrypt(const char *PathImage)
{
	return 0;
}
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) const char* GetErrorString()
{
	return ErrorString;
}
//-----------------------------------------------------------------------------
