#define _CRT_NONSTDC_NO_DEPRECATE

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
//-----------------------------------------------------------------------------
typedef int(__cdecl *FunctionCrypt)(const char *, const char *, const char *);
typedef const char*(__cdecl *FunctionDecrypt)(const char *);
typedef const char*(__cdecl *FunctionGetErrorString)();
//-----------------------------------------------------------------------------
FunctionCrypt Crypt = NULL;
FunctionDecrypt Decrypt = NULL;
FunctionGetErrorString GetErrorString = NULL;
//-----------------------------------------------------------------------------
void Usage();
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef _DEBUG
	HMODULE LibCrypter = LoadLibrary((LPCSTR)"libCrypter.dll");
#else
	HMODULE LibCrypter = LoadLibrary((LPCWSTR)"libCrypter.dll");
#endif
	
	if (LibCrypter) //���� ���������� ������� ��������� � ������
	{
		Crypt = (FunctionCrypt)GetProcAddress(LibCrypter, "Crypt");
		Decrypt = (FunctionDecrypt)GetProcAddress(LibCrypter, "Decrypt");
		GetErrorString = (FunctionGetErrorString)GetProcAddress(LibCrypter, "GetErrorString");

		if (!Crypt || !Decrypt || !GetErrorString)
		{
			printf("Invalid initialize pointers to library \"libCrypter.dll\".\r\n");
		}
	}
	else //���������� �� ���������, ��������� ��������� � �������
	{
		printf("Library \"libCrypter.dll\" not loaded. Error code - %d.\r\n", (int)GetLastError());
		return EXIT_FAILURE;
	}

	char *Mode = strlwr(argv[1]);
	if (!Mode) //���� ����� �� ������, ��������� ��������� � �������
	{
		printf("Mode not specified.\r\n");
		Usage();
		return EXIT_FAILURE;
	}

	char *PathSource = argv[2];
	if (!PathSource) //���� ���� � ��������� ��������� �� ������, ��������� ��������� � �������
	{
		printf("Path source not specified.\r\n");
		Usage();
		return EXIT_FAILURE;
	}

	if (strcmp(Mode, "crypt") == 0) //���� ����� - ����������
	{
		char *PathOutput = argv[3];
		if (!PathOutput) //���� ���� � ��������������� ����������� �� ������, ��������� ��������� � �������
		{
			printf("Invalid path output.\r\n");
			Usage();
			return EXIT_FAILURE;
		}

		char *Message = argv[4];
		if (!Message) //���� ��������� �� �������, ��������� ��������� � �������
		{
			printf("Message not specified.\r\n");
			Usage();
			return EXIT_FAILURE;
		}

		if (Crypt(PathSource, PathOutput, Message) == 1) //���� ���������� �� ���������, ��������� ��������� � �������
		{
			printf("%s\r\n", GetErrorString());
			return EXIT_FAILURE;
		}
	}
	else if (strcmp(Mode, "decrypt") == 0) //���� ����� - ������������
	{
		const char *DecryptedMessage = Decrypt(PathSource);
		if (DecryptedMessage)
		{
			printf("%s\r\n", DecryptedMessage);
			return EXIT_FAILURE;
		}
		else
		{
			printf("Error decrypting: %s\r\n", GetErrorString());
			return EXIT_FAILURE;
		}
	}
	else //������ ������������ �����
	{
		printf("Invalid mode: %s\r\n", Mode);
		return EXIT_FAILURE;
	}

	FreeLibrary(LibCrypter); //��������� ���������� �� ������
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------
void Usage()
{
	printf("Usage: Crypter [MODE] [IMAGE SOURCE PATH] [IMAGE OUTPUT PATH] [MESSAGE]\r\n");
	printf("Parameters:\r\n");
	printf("   1. Work mode: crypt | decrypt\r\n");
	printf("   2. Source path: path from source image\r\n");
	printf("   3. Output path: path from result crypt image\r\n");
	printf("   4. Message: the message you want to encrypt\r\n");
	printf("Examples:\r\n");
	printf("   Crypter crypt C:\\image.png C:\\crypted_image.png \"hello world\"\r\n");
	printf("   Crypter decrypt C:\\crypted_image.png\r\n");
}
//-----------------------------------------------------------------------------
