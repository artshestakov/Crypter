#pragma once
#ifndef _LIBCRYPTER_H_INCLUDED
#define _LIBCRYPTER_H_INCLUDED
//-----------------------------------------------------------------------------
#include <stddef.h>
//-----------------------------------------------------------------------------
typedef struct s_PixelStruct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} PixelStruct;
//-----------------------------------------------------------------------------
typedef long long int rand_t;
//-----------------------------------------------------------------------------
const char* GetErrorString(void);
int Crypt(const char *PathSource, const char *PathOutput, const char *Message);
const char* Decrypt(const char *FilePath);

int ReadFileToMemory(const char *FilePath); //������ ����� � ������
char* PrepareMessage(const char *Message); //���������� ���������
int CheckMessage(const char *MessageComplete, size_t Size); //��������� ���������
int WritePixelsToFile(const char *PathOutput); //������ ���������� ����������� � ����

void InitRandom(rand_t Digit); //������������� �������
rand_t GetRandom(rand_t Minimum, rand_t Maximum); //������������� ��������� ����� ����� � ��������� �� Minimum �� Maximum
size_t GetSizeReserveString(void); //�������� ������ ����������������� ������
int ContainsVector(rand_t Value, size_t VectorSize); //�������� ������������� �������� � "�������"
int FileExist(const char *FilePath); //�������� ������������� �����
//-----------------------------------------------------------------------------
#endif
