#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //Максимальное количество байт для числа
#define SIZE_ERROR_STRING 1024 //Размер строки с описанием ошибки
#define R_OK 1 //Положительный результат
#define R_ERROR 0 //Отрицательный результат
//-----------------------------------------------------------------------------
char *ErrorString;
unsigned int Width = 0;
unsigned int Height = 0;
unsigned int PixelCount = 0;
PixelStruct *Pixels = NULL;
unsigned long *VectorRandom = NULL;
unsigned long Random = 0;
//-----------------------------------------------------------------------------
const char* GetErrorString(void)
{
    return ErrorString;
}
//-----------------------------------------------------------------------------
int Crypt(const char *PathSource, const char *PathOutput, const char *Message)
{
    int Result = ReadFile(PathSource);
    if (Result)
    {
        char *MessageComplete = PrepareMessage(Message);
        Result = MessageComplete ? R_OK : R_ERROR;
        if (Result)
        {
            int MessageSize = strlen(MessageComplete);
            Result = CheckMessage(MessageComplete, MessageSize);
            if (Result)
            {
                for (int i = 0; i < MessageSize; ++i) //Обходим сообщение и вставляем каждый символ в пиксели
                {
                    unsigned long R = GetRandom(1, PixelCount); //Генерируем рандомное число
                    if (ContainsVector(R, MessageSize))
                    {
                        continue;
                    }
                    VectorRandom[i] = R;
                    Pixels[R].A = MessageComplete[i]; //Берем пиксель по рандомному индексу и приравниваем код символа к альфе
                }

                Result = WriteFile(PathOutput);
            }
        }
    }

    Width = 0;
    Height = 0;
    PixelCount = 0;
    if (Pixels)
    {
        free(Pixels);
        Pixels = NULL;
    }
    if (VectorRandom)
    {
        free(VectorRandom);
        VectorRandom = NULL;
    }
    Random = 0;
    return Result;
}
//-----------------------------------------------------------------------------
const char* Decrypt(const char *FilePath)
{
    char *Message = NULL;
    if (ReadFile(FilePath))
    {
        size_t Size = GetSizeReserveString();
        char *Buffer = (char *)malloc(Size);
        for (size_t i = 0; i < Size; ++i)
        {
            Buffer[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        int MessageSize = atoi(Buffer);

        free(Buffer);
        Buffer = NULL;

        Message = (char *)malloc(MessageSize);
        for (int i = 0; i < MessageSize; ++i)
        {
            Message[i] = Pixels[GetRandom(1, PixelCount)].A;
        }
        Message[MessageSize] = '\0';
    }

    Width = 0;
    Height = 0;
    PixelCount = 0;
    if (Pixels)
    {
        free(Pixels);
        Pixels = NULL;
    }
    Random = 0;
    return Message;
}
//-----------------------------------------------------------------------------
int ReadFile(const char *FilePath)
{
    if (!FileExist(FilePath))
    {
        sprintf(ErrorString, "File \"%s\" not exist.", FilePath);
        return R_ERROR;
    }

    unsigned char* Image = NULL;
    unsigned Error = lodepng_decode32_file(&Image, &Width, &Height, FilePath);
    if (Error)
    {
        sprintf(ErrorString, "Error decode file \"%s\": %s", FilePath, lodepng_error_text(Error));
        return R_ERROR;
    }
    
    PixelCount = Width * Height;
    Pixels = (PixelStruct *)malloc(sizeof(PixelStruct *) * PixelCount);

    size_t Index = 0;
    for (unsigned int i = 0; i < PixelCount; ++i)
    {
        PixelStruct Pixel;
        Pixel.R = Image[Index];
        ++Index;
        Pixel.G = Image[Index];
        ++Index;
        Pixel.B = Image[Index];
        ++Index;
        Pixel.A = Image[Index];
        ++Index;
        Pixels[i] = Pixel;
    }

    free(Image);
    Image = NULL;

    char Char[MAX_CHAR_INT];
    sprintf(Char, "%d%d", Width, Height); //Переводим ширину и высоту в строку
    InitRandom(atoi(Char)); //Инициализируем рандом
    memset(Char, 0, sizeof(Char));
    
    for (int i = 0; i < 4; ++i) //Собираем число инициализации первого рандома: последняя цифра каждого нового числа
    {
        char Temp[MAX_CHAR_INT];
        int SizeTemp = sprintf(Temp, "%lu", GetRandom(0, ULONG_MAX)); //Приводим рандомное число к строке
        Char[i] = Temp[SizeTemp - 1]; //Берем последнее число из рандома и запоминаем его
    }

    InitRandom(atoi(Char)); //Снова инициализируем рандом новым числом
    return R_OK;
}
//-----------------------------------------------------------------------------
char* PrepareMessage(const char *Message)
{
    //Запоминаем размер сообщения
    char MessageSize[MAX_CHAR_INT];
    sprintf(MessageSize, "%d", strlen(Message));

    //Создаем строку, которая будет содержать размер сообщения
    size_t Size = GetSizeReserveString(); //Запоминаем размер строки с числом пикселей
    char StringReserve[MAX_CHAR_INT]; //Выделяем память равную размеру строки с пикселями
    StringReserve[Size] = '\0'; //Обрезаем лишний мусор
    memset(StringReserve, '0', Size); //Заполняем строку символами нуля

    //Формируем размер в зарезервированной строке
    size_t Index = strlen(MessageSize);
    size_t I = strlen(StringReserve);
    while (Index != 0)
    {
        --Index; --I;
        StringReserve[I] = MessageSize[Index];
    }

    Size = strlen(StringReserve) + strlen(Message);
    char *MessageComplete = (char *)malloc(Size);
    MessageComplete[Size] = '\0';
    sprintf(MessageComplete, "%s%s", StringReserve, Message);
    return MessageComplete;
}
//-----------------------------------------------------------------------------
int CheckMessage(const char *MessageComplete, size_t Size)
{
    if (strlen(MessageComplete) >= PixelCount) //Если размер сообщения больше или равен количеству пикселей - изображение слишком маленькое для этого сообщения
    {
        sprintf(ErrorString, "This image is too small for your message.");
        return R_ERROR;
    }

    for (size_t i = 0; i < Size; ++i) //Обход сообщения и проверка каждого символа на валидность
    {
        char Char = MessageComplete[i];
        if (Char < 32 && Char > 255)
        {
            sprintf(ErrorString, "Invalid char: %c. Ascii code: %d.", Char, Char);
            return R_ERROR;
        }
    }

    //Создаем массив для временного хранения рандомных чисел
    VectorRandom = VectorRandom = (unsigned long *)malloc(sizeof(unsigned long) * Size);
    memset(VectorRandom, 0, Size);
    return R_OK;
}
//-----------------------------------------------------------------------------
int WriteFile(const char *PathOutput)
{
    unsigned char* Image = (unsigned char *)malloc(PixelCount * 4);
    
    size_t Index = 0;
    for (unsigned long i = 0; i < PixelCount; ++i)
    {
        PixelStruct Pixel = Pixels[i];
        Image[Index] = Pixel.R;
        ++Index;
        Image[Index] = Pixel.G;
        ++Index;
        Image[Index] = Pixel.B;
        ++Index;
        Image[Index] = Pixel.A;
        ++Index;
    }

    unsigned int Error = lodepng_encode32_file(PathOutput, Image, Width, Height);

    free(Image);
    Image = NULL;

    if (Error)
    {
        sprintf(ErrorString, "Error encode file \"%s\": %s", PathOutput, lodepng_error_text(Error));
        return R_ERROR;
    }

    return 1;
}
//-----------------------------------------------------------------------------
void InitRandom(unsigned long Digit)
{
    Random = Digit;
}
//-----------------------------------------------------------------------------
unsigned long GetRandom(unsigned long Minimum, unsigned long Maximum)
{
    Random ^= (Random << 21);
    Random ^= (Random >> 35);
    Random ^= (Random << 4);
    return Minimum + Random % Maximum;
}
//-----------------------------------------------------------------------------
size_t GetSizeReserveString(void)
{
    //Переводим размер пикселей в строку
    char PixelCountString[MAX_CHAR_INT];
    sprintf(PixelCountString, "%d", PixelCount);
    return strlen(PixelCountString);
}
//-----------------------------------------------------------------------------
int ContainsVector(unsigned long Value, size_t MessageSize)
{
    int Result = R_ERROR;
    for (size_t i = 0; i < MessageSize; ++i)
    {
        if (VectorRandom[i] == Value)
        {
            Result = R_OK;
            break;
        }
    }
    return Result;
}
//-----------------------------------------------------------------------------
int FileExist(const char *FilePath)
{
    FILE *File = fopen(FilePath, "rb");
    int Result = File ? R_OK : R_ERROR;
    if (File)
    {
        Result = fclose(File) == 0 ? R_OK : R_ERROR;
        if (!Result)
        {
            sprintf(ErrorString, "Error close file at checking exist: %s.", strerror(errno));
        }
    }
    else
    {
        Result = errno == ENOENT ? R_ERROR : R_OK;
    }
    return Result;
}
//-----------------------------------------------------------------------------
