#include "StdAfx.h"
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //������������ ���������� ���� ��� �����
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
_Bool Crypt(const char *PathSource, const char *PathOutput, const char *Message)
{
    _Bool Result = ReadFileToMemory(PathSource); //������ �����
    if (Result) //���� ������� ��������
    {
        char *MessageComplete = PrepareMessage(Message); //���������� ���������
        Result = MessageComplete ? true : false;
        if (Result) //��������� ������� ������������
        {
            size_t MessageSize = strlen(MessageComplete);
            Result = CheckMessage(MessageComplete, MessageSize); //�������� ���������
            if (Result) //��������� ������ ��������
            {
                //������� ������ ��� ���������� �������� ��������� �����
                size_t VectorSize = MessageSize + BEGIN_RANDOM_SIZE; //� ������� ��������� ����������� ����� ��������� ��������� ������ �������
                VectorRandom = (rand_t *)malloc(sizeof(rand_t) * VectorSize);
                memset(VectorRandom, 0, VectorSize); //�������� ������
                size_t Index = 0; //������ ���������� �������� � �������

                char Temp[MAX_CHAR_INT];
                sprintf(Temp, "%d%d", Width, Height); //��������� ������ � ������ � ������
                InitRandom(atoi(Temp)); //�������������� ������
                memset(Temp, 0, MAX_CHAR_INT); //������� ������

                rand_t Randoms[BEGIN_RANDOM_SIZE]; //������ ������ ������� �������� �������� ���������� �������
                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i) //������� BEGIN_RANDOM_SIZE ��������� �������� � ������� ��������
                {
                    rand_t R = GetRandom(1, PixelCount);
                    if (ContainsVector(R, VectorSize)) //�������� ������������� ������ ����� � "�������"
                    {
                        --i; //����� R ��� ���� �������������, ������� �������� ����� ����� ��������� �������, �� ��� ���� ��������� �������� i �� �������, ����� �� �������� ������� ������ ���������
                        continue;
                    }
                    else
                    {
                        VectorRandom[Index] = R;
                        ++Index;
                    }
                    Randoms[i] = R;
                }

                //�������������� ������ ��������� �������� �������
                sprintf(Temp, "%d", (int)time(NULL));
                InitRandom(atoi(Temp));
                memset(Temp, 0, MAX_CHAR_INT);

                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i)
                {
                    char Char[2];
                    sprintf(Char, "%d", (int)GetRandom(1, 9));
                    Pixels[Randoms[i]].A = Char[0];
                    Temp[i] = Char[0];
                }

                InitRandom(atoi(Temp));

                for (size_t i = 0; i < MessageSize; ++i) //������� ��������� � ��������� ������ ������ � �������
                {
                    rand_t R = GetRandom(1, PixelCount); //���������� ��������� �����
                    if (ContainsVector(R, VectorSize)) //�������� ������������� ������ ����� � "�������"
                    {
                        --i; //����� R ��� ���� �������������, ������� �������� ����� ����� ��������� �������, �� ��� ���� ��������� �������� i �� �������, ����� �� �������� ������� ������ ���������
                        continue;
                    }
                    else
                    {
                        VectorRandom[Index] = R;
                        ++Index;
                    }
                    Pixels[R].A = MessageComplete[i]; //����� ������� �� ���������� ������� � ������������ ��� ������� � �����
                }
                Result = WritePixelsToFile(PathOutput); //������ �������� � ����
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
    if (ReadFileToMemory(FilePath)) //������ ����
    {
        char Temp[MAX_CHAR_INT];
        sprintf(Temp, "%d%d", Width, Height); //��������� ������ � ������ � ������
        InitRandom(atoi(Temp)); //�������������� ������
        memset(Temp, 0, MAX_CHAR_INT); //������� ������

        //�������� ����������� �������� � ������� �������� ����������������� ����� ��� �������
        for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i)
        {
            Temp[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        InitRandom(atoi(Temp));

        size_t Size = GetSizeReserveString();
        char *Buffer = (char *)malloc(Size);
        for (size_t i = 0; i < Size; ++i) //��������� ����������������� ������
        {
            Buffer[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        int MessageSize = atoi(Buffer); //��������� ����������������� ������ � ����� ����� (������ ���������)

        free(Buffer);
        Buffer = NULL;

        if (MessageSize) //���� ������ ��������� ������ ���� - ��������� ��������� �� ��������
        {
            Message = (char *)malloc(MessageSize);
            for (int i = 0; i < MessageSize; ++i)
            {
                rand_t R = GetRandom(1, PixelCount);
                Message[i] = Pixels[R].A;
            }
            Message[MessageSize] = '\0';
        }
        else //������ ��������� ����� ���� - ������: �������� ��������� � �� ���������� � �����������
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
_Bool ReadFileToMemory(const char *FilePath)
{
    if (!FilePath)
    {
        sprintf(ErrorString, "File path is empty.");
        return false;
    }

    if (!FileExist(FilePath))
    {
        sprintf(ErrorString, "File \"%s\" not exist.", FilePath);
        return false;
    }

    unsigned char* Image = NULL;
    unsigned Error = lodepng_decode32_file(&Image, &Width, &Height, FilePath);
    if (Error)
    {
        sprintf(ErrorString, "Error decode file \"%s\": %s", FilePath, lodepng_error_text(Error));
        return false;
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

    return true;
}
//-----------------------------------------------------------------------------
char* PrepareMessage(const char *Message)
{
    //���������� ������ ���������
    char MessageSize[MAX_CHAR_INT];
    sprintf(MessageSize, "%zu", strlen(Message));

    //������� ������, ������� ����� ��������� ������ ���������
    size_t Size = GetSizeReserveString(); //���������� ������ ������ � ������ ��������
    char StringReserve[MAX_CHAR_INT]; //�������� ������ ������ ������� ������ � ���������
    StringReserve[Size] = '\0'; //�������� ������ �����
    memset(StringReserve, 40, Size); //��������� ������ ��������� ���� (40 �������� ����� ����)

    //��������� ������ � ����������������� ������
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
_Bool CheckMessage(const char *MessageComplete, size_t Size)
{
    if (strlen(MessageComplete) >= PixelCount) //���� ������ ��������� ������ ��� ����� ���������� �������� - ����������� ������� ��������� ��� ����� ���������
    {
        sprintf(ErrorString, "This image is too small for your message.");
        return false;
    }

    for (size_t i = 0; i < Size; ++i) //����� ��������� � �������� ������� ������� �� ����������
    {
        char Char = MessageComplete[i];
        if (Char < 32 && Char > 255)
        {
            sprintf(ErrorString, "Invalid char: %c. Ascii code: %d.", Char, Char);
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
_Bool WritePixelsToFile(const char *PathOutput)
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
        return false;
    }

    return true;
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
    //��������� �������������� 4293: MSVC ������� ��� ����� ������ �� 35 ����� ������� ������������� ���������,
    //�� ��� ������ 35 - ������������� ��� ������� ������� �����. �� ���� ��!
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
    //��������� ������ �������� � ������ � ���������� ������ ���� ������
    char PixelCountString[MAX_CHAR_INT];
    sprintf(PixelCountString, "%d", PixelCount);
    return strlen(PixelCountString);
}
//-----------------------------------------------------------------------------
_Bool ContainsVector(rand_t Value, size_t VectorSize)
{
    _Bool Result = false;
    for (size_t i = 0; i < VectorSize; ++i)
    {
        if (VectorRandom[i] == Value)
        {
            Result = true;
            break;
        }
    }
    return Result;
}
//-----------------------------------------------------------------------------
_Bool FileExist(const char *FilePath)
{
    FILE *File = fopen(FilePath, "rb");
    _Bool Result = File ? true : false;
    if (File)
    {
        Result = fclose(File) == 0 ? true : false;
        if (!Result)
        {
            sprintf(ErrorString, "Error close file at checking exist: %s.", strerror(errno));
        }
    }
    else
    {
        Result = errno == ENOENT ? false : true;
    }
    return Result;
}
//-----------------------------------------------------------------------------
