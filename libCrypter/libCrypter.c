#include "StdAfx.h"
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //Максимальное количество байт для числа
#define R_OK 1 //Положительный результат
#define R_ERROR 0 //Отрицательный результат
#define BEGIN_RANDOM_SIZE 4
//-----------------------------------------------------------------------------
char ErrorString[1024];
unsigned int Width = 0;
unsigned int Height = 0;
unsigned int PixelCount = 0;
PixelStruct *Pixels = NULL;
rand_t *VectorRandom = NULL;
rand_t Random = 0;
//-----------------------------------------------------------------------------
const char* GetErrorString(void)
{
    return ErrorString;
}
//-----------------------------------------------------------------------------
int Crypt(const char *PathSource, const char *PathOutput, const char *Message)
{
    int Result = ReadFileToMemory(PathSource); //Чтение файла
    if (Result) //Файл успешно прочитан
    {
        char *MessageComplete = PrepareMessage(Message); //Подготовка сообщения
        Result = MessageComplete ? R_OK : R_ERROR;
        if (Result) //Сообщение успешно подготовлено
        {
            size_t MessageSize = strlen(MessageComplete);
            Result = CheckMessage(MessageComplete, MessageSize); //Проверка сообщения
            if (Result) //Сообщение прошло проверку
            {
                //Создаем вектор для временного хранения рандомных чисел
                size_t VectorSize = MessageSize + BEGIN_RANDOM_SIZE; //К размеру сообщения обязательно нужно прибавить начальный размер рандома
                VectorRandom = (rand_t *)malloc(sizeof(rand_t) * VectorSize);
                memset(VectorRandom, 0, VectorSize); //Обнуляем вектор
                size_t Index = 0; //Индекс последнего значения в векторе

                char Temp[MAX_CHAR_INT];
                sprintf(Temp, "%d%d", Width, Height); //Переводим ширину и высоту в строку
                InitRandom(atoi(Temp)); //Инициализируем рандом
                memset(Temp, 0, MAX_CHAR_INT); //Очищаем строку

                rand_t Randoms[BEGIN_RANDOM_SIZE]; //Массив первых четырех индексов хранения начального рандома
                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i) //Создаем BEGIN_RANDOM_SIZE случайных индексов в массиве пикселей
                {
                    rand_t R = GetRandom(1, PixelCount);
                    if (ContainsVector(R, VectorSize)) //Проверка существования такого числа в "векторе"
                    {
                        --i; //Число R уже было сгенерировано, поэтому итерацию цикла нужно выполнить сначала, но при этом уменьшить итератор i на единицу, чтобы не нарушать порядок обхода сообщения
                        continue;
                    }
                    else
                    {
                        VectorRandom[Index] = R;
                        ++Index;
                    }
                    Randoms[i] = R;
                }

                //Инициализируем рандом значением текущего времени
                sprintf(Temp, "%d", time(NULL));
                InitRandom(atoi(Temp));
                memset(Temp, 0, MAX_CHAR_INT);

                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i)
                {
                    char Char[2];
                    rand_t R = GetRandom(1, 9);
                    sprintf(Char, "%lu", R);
                    Pixels[Randoms[i]].A = Char[0];
                    Temp[i] = Char[0];
                }

                InitRandom(atoi(Temp));

                for (size_t i = 0; i < MessageSize; ++i) //Обходим сообщение и вставляем каждый символ в пиксели
                {
                    rand_t R = GetRandom(1, PixelCount); //Генерируем рандомное число
                    if (ContainsVector(R, VectorSize)) //Проверка существования такого числа в "векторе"
                    {
                        --i; //Число R уже было сгенерировано, поэтому итерацию цикла нужно выполнить сначала, но при этом уменьшить итератор i на единицу, чтобы не нарушать порядок обхода сообщения
                        continue;
                    }
                    else
                    {
                        VectorRandom[Index] = R;
                        ++Index;
                    }
                    Pixels[R].A = MessageComplete[i]; //Берем пиксель по рандомному индексу и приравниваем код символа к альфе
                }
                Result = WritePixelsToFile(PathOutput); //Запись пикселей в файл
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
    if (ReadFileToMemory(FilePath)) //Читаем файл
    {
        char Temp[MAX_CHAR_INT];
        sprintf(Temp, "%d%d", Width, Height); //Переводим ширину и высоту в строку
        InitRandom(atoi(Temp)); //Инициализируем рандом
        memset(Temp, 0, MAX_CHAR_INT); //Очищаем строку

        //Получаем координатиы пекселей в которых хранится инициализационное число для рандома
        for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i)
        {
            Temp[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        InitRandom(atoi(Temp));

        size_t Size = GetSizeReserveString();
        char *Buffer = (char *)malloc(Size);
        for (size_t i = 0; i < Size; ++i) //Формируем зарезервированную строку
        {
            Buffer[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        int MessageSize = atoi(Buffer); //Переводим зарезервированную строку в целое число (размер сообщения)

        free(Buffer);
        Buffer = NULL;

        if (MessageSize) //Если размер сообщения больше нуля - извлекаем сообщение из пикселей
        {
            Message = (char *)malloc(MessageSize);
            for (int i = 0; i < MessageSize; ++i)
            {
                rand_t R = GetRandom(1, PixelCount);
                Message[i] = Pixels[R].A;
            }
            Message[MessageSize] = '\0';
        }
        else //Размер сообщения равен нулю - ошибка: возможно сообщение и не содержится в изображении
        {
            sprintf(ErrorString, "The message is not contained in this image.");
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
    Random = 0;
    return Message;
}
//-----------------------------------------------------------------------------
int ReadFileToMemory(const char *FilePath)
{
    if (!FilePath)
    {
        sprintf(ErrorString, "File path is empty.");
        return R_ERROR;
    }

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
    memset(StringReserve, 40, Size); //Заполняем строку символами нуля (40 является кодом нуля)

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
    return R_OK;
}
//-----------------------------------------------------------------------------
int WritePixelsToFile(const char *PathOutput)
{
    unsigned char* Image = (unsigned char *)malloc(PixelCount * 4);
    
    size_t Index = 0;
    for (unsigned int i = 0; i < PixelCount; ++i)
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

    return R_OK;
}
//-----------------------------------------------------------------------------
void InitRandom(rand_t Digit)
{
    Random = Digit;
}
//-----------------------------------------------------------------------------
rand_t GetRandom(rand_t Minimum, rand_t Maximum)
{
    Random ^= (Random << 21);
    //Подавляем предупреждение 4293: MSVC считает что сдвиг вправо на 35 может вызвать непоределнное поведение,
    //по его мнению 35 - отрицательное или слишком большое число. Ну бред же!
#pragma warning (disable: 4293)
    Random ^= (Random >> 35);
#pragma warning (default: 4293)
    Random ^= (Random << 4);
    rand_t Result = Minimum + Random % Maximum;
    if (Result < 0)
    {
        Result = -Result;
    }
    return Result;
}
//-----------------------------------------------------------------------------
size_t GetSizeReserveString(void)
{
    //Переводим размер пикселей в строку и возвращаем размер этой строки
    char PixelCountString[MAX_CHAR_INT];
    sprintf(PixelCountString, "%d", PixelCount);
    return strlen(PixelCountString);
}
//-----------------------------------------------------------------------------
int ContainsVector(rand_t Value, size_t VectorSize)
{
    int Result = R_ERROR;
    for (size_t i = 0; i < VectorSize; ++i)
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
