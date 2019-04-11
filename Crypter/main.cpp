#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
//-----------------------------------------------------------------------------
typedef int(__cdecl *FunctionCrypt)(const char *, const char *, const char *, const char *);
typedef int(__cdecl *FunctionDecrypt)(const char *, const char *);
typedef const char*(__cdecl *FunctionGetErrorString)();
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	HMODULE LibCrypter = LoadLibrary("libCrypter.dll");
	if (!LibCrypter)
	{
		printf("Library \"libCrypter.dll\" not loaded. Error code - %d.\r\n", (int)GetLastError());
		return EXIT_FAILURE;
	}
	
	FunctionCrypt Crypt = (FunctionCrypt)GetProcAddress(LibCrypter, "Crypt");
	FunctionDecrypt Decrypt = (FunctionDecrypt)GetProcAddress(LibCrypter, "Decrypt");
	FunctionGetErrorString GetErrorString = (FunctionGetErrorString)GetProcAddress(LibCrypter, "GetErrorString");

	int Result = Crypt("PNG", "G:\\image.png", "G:\\output.png", "message");
	if (!Result)
	{
		printf("%s\r\n", GetErrorString());
		return EXIT_FAILURE;
	}
	
	Result = Decrypt("123", "");
	if (!Result)
	{
		printf("%s\r\n", GetErrorString());
		return EXIT_FAILURE;
	}

	FreeLibrary(LibCrypter);
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------
