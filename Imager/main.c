#include "libCrypter_global.h"
#include <string.h>
#include <stdio.h>
#include <Windows.h>
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int Result = CryptMessage("G:\\Logo.png", "G:\\1.png", "Hello world!");
    if (Result)
    {
        const char *StringResult = DecryptMessage("G:\\1.png");
        if (StringResult)
        {
            if (strcmp(StringResult, "Hello world!") == 0)
            {
                printf("OK\n");
            }
            else
            {
                printf("Result string (%s) not equal to string \"%s\"\n", StringResult, "Hello world!");
            }
        }
        else
        {
            printf("String result is null.\n");
        }
    }

    if (!Result)
    {
        printf("Error: %s\n", GetError());
    }

    return 0;
}
//-----------------------------------------------------------------------------
