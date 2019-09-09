#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libCrypter_global.h"
//-----------------------------------------------------------------------------
void Usage()
{
    printf("\nCrypter application for usage libCrypter.\n");
    printf("Usage: Crypter [MODE] [PATH_SOURCE] [PATH_OUTPUT] [MESSAGE]\n\n");
    printf("Mode:\n");
    printf("-c - crypt mode\n");
    printf("-d - decrypt mode\n");
    printf("-h - show this message\n\n");
    printf("Examples:\n");
#ifdef WIN32
    printf("Crypter -c C:\\Image.png C:\\Image_Output.png \"Hello world!\"\n");
    printf("Crypter -d C:\\Image_Outout.png\n");
#else
    printf("Crypter -c /home/[USER_NAME]/Image.png /home/[USER_NAME]/Image_Output.png \"Hello world!\"\n");
    printf("Crypter -d /home/[USER_NAME]/Image_Outout.png\n");
#endif
}
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    (void)argc;
    
    const char *Mode = NULL; //����� ������
    const char *PathSource = NULL; //���� � ��������� �����������
    const char *PathOutput = NULL; //���� � ��������� �����������
    const char *Message = NULL; //���������

    Mode = argv[1];
    if (strlen(Mode ? Mode : "") == 0) //���� ����� ������ �� ������
    {
        printf("Error: not specified mode.\n");
        Usage();
        return EXIT_FAILURE;
    }

    if (strcmp(Mode, "-c") == 0) //���� ������ ����� �����������
    {
        PathSource = argv[2];
        if (strlen(PathSource ? PathSource : "") == 0)
        {
            printf("Error: not specified path source.\n");
            Usage();
            return EXIT_FAILURE;
        }

        PathOutput = argv[3];
        if (strlen(PathOutput ? PathOutput : "") == 0)
        {
            printf("Error: not specified path output.\n");
            Usage();
            return EXIT_FAILURE;
        }

        Message = argv[4];
        if (strlen(Message ? Message : "") == 0)
        {
            printf("Error: not specified message.\n");
            Usage();
            return EXIT_FAILURE;
        }

        if (CryptMessage(PathSource, PathOutput, Message) == true)
        {
            printf("Crypted is success.\n");
        }
        else
        {
            printf("Error: %s\n", GetError());
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(Mode, "-d") == 0) //���� ������ ����� ��������������
    {
        PathSource = argv[2];
        if (strlen(PathSource ? PathSource : "") == 0)
        {
            printf("Error: not specified path source.\n");
            Usage();
            return EXIT_FAILURE;
        }

        const char *Result = DecryptMessage(PathSource);
        if (Result)
        {
            printf("Decrypted is success.\nMessage: %s\n", Result);
        }
        else
        {
            printf("Error: %s\n", GetError());
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(Mode, "-h") == 0) //���� ������ ������ ����������� ������
    {
        Usage();
        return EXIT_SUCCESS;
    }
    else //����������� ����� - ������
    {
        printf("Error: invalid mode \"%s\"\n", Mode);
        Usage();
        return EXIT_FAILURE;
    }

#ifdef _DEBUG //��� �������
    system("PAUSE");
#endif

    return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------
