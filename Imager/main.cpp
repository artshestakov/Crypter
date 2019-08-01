#include "libCrypter_global.h"
#include <string.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    Crypt("G:\\Logo.png", "G:\\1.png", "Hello world!");

    return 0;
}
//-----------------------------------------------------------------------------
