#pragma once
#ifndef _LIBCRYPTER_H_INCLUDED
#define _LIBCRYPTER_H_INCLUDED
//-----------------------------------------------------------------------------
#include <stddef.h>
//-----------------------------------------------------------------------------
//��������� ����������� �������
typedef struct pixel_struct_t
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
} PixelStruct;
//-----------------------------------------------------------------------------
typedef long long int rand_t; //��� ��� ���������� �����
//-----------------------------------------------------------------------------

/**
* ��������� ���������� �������� ������.
* @return ���������� ����������� ��������� �� ������.
*/
const char* GetErrorString(void);

/**
* ����������� ��������� � �����������.
* @param PathSource - ���� � ��������� �����������.
* @param PathOutput - ���� � �����������, � ������� ����� ����������� ����������� ���������.
* @param Message - ����������� ��������� �� ������, ������� �������� ���������� ����������.
* @return ���������� 1 � ������ ������, ����� - 0.
*/
_Bool Crypt(const char *PathSource, const char *PathOutput, const char *Message);

/**
* ������������� ��������� �� �����������.
* @param FilePath - ���� � �����������, ��������� �� �������� ���������� ��������.
* @return ���������� ����������� ��������� �� ������, � ������� ���������� �������������� ���������.
*/
const char* Decrypt(const char *FilePath);

/**
* ������ ����� � ������������ � ������.
* @param FilePath - ���� � ����� � ������������.
* @return ���������� 1 � ������ ��������� ������, ����� - 0.
*/
_Bool ReadFileToMemory(const char *FilePath);

/**
* ���������� ��������� � �����������.
* @param Message - ����������� ��������� �� ������, ������� �������� ���������� ����������.
* @return ���������� ��������� �� ������, � ������� ���������� ������� �����������.
*/
char* PrepareMessage(const char *Message);

/**
* �������� ��������� �� ���������� � �����������.
* @param MessageComplete - ����������� ��������� �� ������, ������� �������� ���������� ����������.
* @param Size ������ - ���������.
* @return ���������� ��������� �� ������, � ������� ���������� ������� �����������.
*/
_Bool CheckMessage(const char *MessageComplete, size_t Size);

/**
* ������ �������� ����������� � ����.
* @param PathOutput - ���� � ��������� ����� � ������������.
* @return ���������� 1 � ������ ������, ����� - 0.
*/
_Bool WritePixelsToFile(const char *PathOutput);

/**
* ������������� �������.
* @param Digit - �����, ������� ����� ������������� ��������� ��������� �����.
*/
void InitRandom(rand_t Digit);

/**
* �������� ����� ��������� �����.
* @param Minimum - ����������� ������� ���������� �����.
* @param Maximum - ������������ ������� ���������� �����.
*/
rand_t GetRandom(rand_t Minimum, rand_t Maximum);

/**
* �������� ������ ����������������� ������.
* @return ���������� ������ ����������������� ������.
*/
size_t GetSizeReserveString(void);

/**
* �������� ������������� �������� � "�������".
* @param Value - ����������� ��������.
* @param VectorSize - ������ "�������".
* @return ���������� 1 � ������ ������, ����� - 0.
*/
_Bool ContainsVector(rand_t Value, size_t VectorSize);

/**
* �������� ������������� ����� � �������� �������.
* @param FilePath - ���� � �����.
* @return ���������� 1 � ������ ������������� �����, ����� - 0.
*/
_Bool FileExist(const char *FilePath); //�������� ������������� �����
//-----------------------------------------------------------------------------
#endif
