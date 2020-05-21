#include "libCrypter_global.h"
#include "libCrypter.h"
//-----------------------------------------------------------------------------
int CryptMessage(const char *PathSource, const char *PathOutput, const char *Message)
{
    return Crypt(PathSource, PathOutput, Message);
}
//-----------------------------------------------------------------------------
const char* DecryptMessage(const char *FilePath)
{
    return Decrypt(FilePath);
}
//-----------------------------------------------------------------------------
const char* GetError(void)
{
    return GetErrorString();
}
//-----------------------------------------------------------------------------
int GenerateImage(const char *PathImage, const char *String)
{
	return Generate(PathImage, String);
}
//-----------------------------------------------------------------------------
