#include "Imager.h"
#include <string.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int Count = 1000;
    while (--Count)
    {
        const char Message[] = "Hello world!";
        const char *ResultString = nullptr;

        Imager imager;
        bool Result = imager.Crypt("G:\\Logo.png", "G:\\1.png", Message);
        if (Result)
        {
            ResultString = imager.Decrypt("G:\\1.png");
        }

        if (Result && ResultString)
        {
            if (strcmp(Message, ResultString) == 0)
            {
                printf("OK %d\n", Count);
            }
        }
        else
        {
            printf("ERROR: %s\n", imager.GetErrorString());
            break;
        }
    }

    return 0;
}
//-----------------------------------------------------------------------------
