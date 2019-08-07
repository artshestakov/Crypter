#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libCrypter_global.h"
//-----------------------------------------------------------------------------
void Usage()
{
    printf("\nCrypter application for usage libCrypter.\n");
    printf("Usage: Crypter [MODE] [PATH_SOURCE] [PATH_OUTPUT] [MESSAGE]\n\n");
    printf("Mode:\n");
    printf("-c - crypt\n");
    printf("-d - decrypt\n");
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
    (void)argv;

    const char *Mode = NULL; //Режим работы
    const char *PathSource = NULL; //Путь к исходному изображению
    const char *PathOutput = NULL; //Путь к выходному изображению
    const char *Message = NULL; //Сообщение

    Mode = argv[1];
    if (strlen(Mode ? Mode : "") == 0) //Если режим работы не указан
    {
        printf("Error: not specified mode.\n");
        Usage();
        return EXIT_FAILURE;
    }

    if (strcmp(Mode, "-c") == 0) //Если указан режим шифрования
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

        if (CryptMessage(PathSource, PathOutput, Message))
        {
            printf("Crypted is success.\n");
        }
        else
        {
            printf("Error: %s\n", GetError());
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(Mode, "-d") == 0) //Если указан режим дешифрования
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
    else if (strcmp(Mode, "-h") == 0) //Если указан запрос отображения помощи
    {
        Usage();
        return EXIT_SUCCESS;
    }
    else //Неизвестный режим - ошибка
    {
        printf("Error: invalid mode \"%s\"\n", Mode);
        Usage();
        return EXIT_FAILURE;
    }

#ifdef _DEBUG //Для отладки
    system("PAUSE");
#endif
    return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------
