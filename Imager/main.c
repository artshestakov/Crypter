#include "libCrypter_global.h"
#include <string.h>
#include <stdio.h>
#include <Windows.h>
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    WIN32_FIND_DATA FindData;
    HANDLE Handle = FindFirstFile("G:\\images\\*.png", &FindData);
    if (Handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            char PathSource[MAX_PATH] = "G:\\images\\";
            strcat(PathSource, FindData.cFileName);

            char PathOutput[MAX_PATH] = "G:\\images2\\";
            strcat(PathOutput, FindData.cFileName);

            int Result = CryptMessage(PathSource, PathOutput, FindData.cFileName);
            if (Result)
            {
                const char *StringResult = DecryptMessage(PathOutput);
                if (StringResult)
                {
                    if (strcmp(StringResult, FindData.cFileName) == 0)
                    {
                        printf("OK\n");
                    }
                    else
                    {
                        printf("Result string (%s) not equal to string \"%s\"\n", StringResult, FindData.cFileName);
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

        } while (FindNextFile(Handle, &FindData));

        FindClose(Handle);
    }

    return 0;
}
//-----------------------------------------------------------------------------
