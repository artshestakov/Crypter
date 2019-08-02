#pragma once
#ifndef IMAGER_H_INCLUDED
#define IMAGER_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
//-----------------------------------------------------------------------------
typedef struct s_PixelStruct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} PixelStruct;
//-----------------------------------------------------------------------------
const char* GetErrorString(void);
int Crypt(const char *PathSource, const char *PathOutput, const char *Message);
const char* Decrypt(const char *FilePath);

int ReadFile(const char *FilePath);
char* PrepareMessage(const char *Message);
int CheckMessage(const char *MessageComplete, size_t Size);
int WriteFile(const char *PathOutput);

void InitRandom(unsigned long Digit);
unsigned long GetRandom(unsigned long Minimum, unsigned long Maximum);
size_t GetSizeReserveString(void);
int ContainsVector(unsigned long Value, size_t MessageSize);
int FileExist(const char *FilePath);
//-----------------------------------------------------------------------------
#endif
