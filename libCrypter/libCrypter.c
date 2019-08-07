#include "StdAfx.h"
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //������������ ���������� ���� ��� �����
#define R_OK 1 //������������� ���������
#define R_ERROR 0 //������������� ���������
#define BEGIN_RANDOM_SIZE 4
//-----------------------------------------------------------------------------
char ErrorString[1024];
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
    int Result = ReadFile(PathSource); //������ �����
    if (Result) //���� ������� ��������
    {
        char *MessageComplete = PrepareMessage(Message); //���������� ���������
        Result = MessageComplete ? R_OK : R_ERROR;
        if (Result) //��������� ������� ������������
        {
            size_t MessageSize = strlen(MessageComplete);
            Result = CheckMessage(MessageComplete, MessageSize); //�������� ���������
            if (Result) //��������� ������ ��������
            {
                //������� ������ ��� ���������� �������� ��������� �����
                size_t VectorSize = MessageSize + BEGIN_RANDOM_SIZE; //� ������� ��������� ����������� ����� ��������� ��������� ������ �������
                VectorRandom = (unsigned long *)malloc(sizeof(unsigned long) * VectorSize);
                memset(VectorRandom, 0, VectorSize); //�������� ������
                size_t Index = 0; //������ ���������� �������� � �������

                char Temp[MAX_CHAR_INT];
                sprintf(Temp, "%d%d", Width, Height); //��������� ������ � ������ � ������
                InitRandom(atoi(Temp)); //�������������� ������
                memset(Temp, 0, MAX_CHAR_INT); //������� ������

                unsigned long Randoms[BEGIN_RANDOM_SIZE]; //������ ������ ������� �������� �������� ���������� �������
                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i) //������� BEGIN_RANDOM_SIZE ��������� �������� � ������� ��������
                {
                    unsigned long R = GetRandom(1, PixelCount);
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
                sprintf(Temp, "%d", time(NULL));
                InitRandom(atoi(Temp));
                memset(Temp, 0, MAX_CHAR_INT);

                for (size_t i = 0; i < BEGIN_RANDOM_SIZE; ++i)
                {
                    char Char[2];
                    unsigned long R = GetRandom(1, 9);
                    sprintf(Char, "%lu", R);
                    Pixels[Randoms[i]].A = Char[0];
                    Temp[i] = Char[0];
                }

                InitRandom(atoi(Temp));

                for (size_t i = 0; i < MessageSize; ++i) //������� ��������� � ��������� ������ ������ � �������
                {
                    unsigned long R = GetRandom(1, PixelCount); //���������� ��������� �����
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
                Result = WriteFile(PathOutput); //������ �������� � ����
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
    if (ReadFile(FilePath)) //������ ����
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
                unsigned long R = GetRandom(1, PixelCount);
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
int ReadFile(const char *FilePath)
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

    /*
    char Char[MAX_CHAR_INT];
    sprintf(Char, "%d%d", Width, Height); //��������� ������ � ������ � ������
    InitRandom(atoi(Char)); //�������������� ������
    memset(Char, 0, sizeof(Char));
    
    for (int i = 0; i < 4; ++i) //�������� ����� ������������� ������� �������: ��������� ����� ������� ������ �����
    {
        char Temp[MAX_CHAR_INT];
        int SizeTemp = sprintf(Temp, "%lu", GetRandom(0, ULONG_MAX)); //�������� ��������� ����� � ������
        Char[i] = Temp[SizeTemp - 1]; //����� ��������� ����� �� ������� � ���������� ���
    }

    InitRandom(atoi(Char)); //����� �������������� ������ ����� ������
    */
    return R_OK;
}
//-----------------------------------------------------------------------------
char* PrepareMessage(const char *Message)
{
    //���������� ������ ���������
    char MessageSize[MAX_CHAR_INT];
    sprintf(MessageSize, "%d", strlen(Message));

    //������� ������, ������� ����� ��������� ������ ���������
    size_t Size = GetSizeReserveString(); //���������� ������ ������ � ������ ��������
    char StringReserve[MAX_CHAR_INT]; //�������� ������ ������ ������� ������ � ���������
    StringReserve[Size] = '\0'; //�������� ������ �����
    memset(StringReserve, '0', Size); //��������� ������ ��������� ����

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
int CheckMessage(const char *MessageComplete, size_t Size)
{
    if (strlen(MessageComplete) >= PixelCount) //���� ������ ��������� ������ ��� ����� ���������� �������� - ����������� ������� ��������� ��� ����� ���������
    {
        sprintf(ErrorString, "This image is too small for your message.");
        return R_ERROR;
    }

    for (size_t i = 0; i < Size; ++i) //����� ��������� � �������� ������� ������� �� ����������
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

    return R_OK;
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
    //��������� �������������� 4293: MSVC ������� ��� ����� ������ �� 35 ����� ������� ������������� ���������,
    //�� ��� ������ 35 - ������������� ��� ������� ������� �����. �� ���� ��!
#pragma warning (disable: 4293)
    Random ^= (Random >> 35);
#pragma warning (default: 4293)
    Random ^= (Random << 4);
    return Minimum + Random % Maximum;
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
