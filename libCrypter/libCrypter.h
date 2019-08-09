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

int ReadFileV(const char *FilePath); //Чтение файла в память
char* PrepareMessage(const char *Message); //Подготовка сообщения
int CheckMessage(const char *MessageComplete, size_t Size); //Прооверка сообщения
int WriteFileV(const char *PathOutput); //Запись результата кодирования в файл

void InitRandom(rand_t Digit); //Инициализация рандома
rand_t GetRandom(rand_t Minimum, rand_t Maximum); //Сгенерировать случайное целое число в диапазоне от Minimum до Maximum
size_t GetSizeReserveString(void); //Получить размер зарезервированной строки
int ContainsVector(rand_t Value, size_t VectorSize); //Проверка существования значения в "векторе"
int FileExist(const char *FilePath); //Проверка существования файла
//-----------------------------------------------------------------------------
#endif
