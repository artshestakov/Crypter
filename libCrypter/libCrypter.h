#pragma once
#ifndef LIBCRYPTER_H_INCLUDED
#define LIBCRYPTER_H_INCLUDED
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
const char* GetErrorString(void);
int Crypt(const char *PathSource, const char *PathOutput, const char *Message);
const char* Decrypt(const char *FilePath);

int ReadFile(const char *FilePath); //Чтение файла в память
char* PrepareMessage(const char *Message); //Подготовка сообщения
int CheckMessage(const char *MessageComplete, size_t Size); //Прооверка сообщения
int WriteFile(const char *PathOutput); //Запись результата кодирования в файл

void InitRandom(unsigned long Digit); //Инициализация рандома
unsigned long GetRandom(unsigned long Minimum, unsigned long Maximum); //Сгенерировать случайное целое число в диапазоне от Minimum до Maximum
size_t GetSizeReserveString(void); //Получить размер зарезервированной строки
int ContainsVector(unsigned long Value, size_t MessageSize); //Проверка существования значения в "векторе"
int FileExist(const char *FilePath); //Проверка существования файла
//-----------------------------------------------------------------------------
#endif
