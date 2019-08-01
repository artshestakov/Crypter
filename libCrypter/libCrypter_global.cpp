#include "libCrypter_global.h"
#include "libCrypter.h"
//-----------------------------------------------------------------------------
int Crypt(const char *PathSource, const char *PathOutput, const char *Message)
{
    return libCrypter::Crypt(PathSource, PathOutput, Message);
}
//-----------------------------------------------------------------------------
const char* Decrypt(const char *FilePath)
{
    return FilePath;
}
//-----------------------------------------------------------------------------
