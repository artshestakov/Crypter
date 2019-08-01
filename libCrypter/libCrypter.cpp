#define _CRT_SECURE_NO_WARNINGS
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //������������ ���������� ���� ��� �����
#define SIZE_ERROR_STRING 1024 //������ ������ � ��������� ������
//-----------------------------------------------------------------------------
libCrypter::libCrypter()
    : ErrorString(new char[SIZE_ERROR_STRING]),
    Width(0),
    Height(0),
    PixelCount(0),
    Pixels(nullptr),
    Random(0),
    VectorRandom(nullptr)
{
    memset(ErrorString, 0, sizeof(char));
}
//-----------------------------------------------------------------------------
libCrypter::~libCrypter()
{
    delete[] ErrorString;
    ErrorString = nullptr;
}
//-----------------------------------------------------------------------------
const char* libCrypter::GetErrorString() const
{
    return ErrorString;
}
//-----------------------------------------------------------------------------
bool libCrypter::Crypt(const char *PathSource, const char *PathOutput, const char *Message)
{
    bool Result = ReadFile(PathSource);
    if (Result)
    {
        char *MessageComplete = PrepareMessage(Message);
        Result = MessageComplete ? true : false;
        if (Result)
        {
            size_t MessageSize = strlen(MessageComplete);
            Result = CheckMessage(MessageComplete, MessageSize);
            if (Result)
            {
                for (size_t i = 0; i < MessageSize; ++i) //������� ��������� � ��������� ������ ������ � �������
                {
                    unsigned long R = GetRandom(1, PixelCount); //���������� ��������� �����
                    if (ContainsVector(R, MessageSize))
                    {
                        continue;
                    }
                    VectorRandom[i] = R;
                    Pixels[R].A = MessageComplete[i]; //����� ������� �� ���������� ������� � ������������ ��� ������� � �����
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
        delete[] Pixels;
        Pixels = nullptr;
    }
    if (VectorRandom)
    {
        delete[] VectorRandom;
        VectorRandom = nullptr;
    }
    Random = 0;
    return Result;
}
//-----------------------------------------------------------------------------
const char* libCrypter::Decrypt(const char *FilePath)
{
    char *Message = nullptr;
    if (ReadFile(FilePath))
    {
        size_t Size = GetSizeReserveString();
        char *Buffer = new char[Size];
        for (size_t i = 0; i < Size; ++i)
        {
            Buffer[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        size_t MessageSize = atoi(Buffer);

        delete[] Buffer;
        Buffer = nullptr;

        Message = new char[MessageSize];
        for (size_t i = 0; i < MessageSize; ++i)
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
        delete[] Pixels;
        Pixels = nullptr;
    }
    Random = 0;
    return Message;
}
//-----------------------------------------------------------------------------
bool libCrypter::ReadFile(const char *FilePath)
{
    if (!FileExist(FilePath))
    {
        sprintf(ErrorString, "File \"%s\" not exist.", FilePath); return false;
    }

    unsigned char* Image = nullptr;
    unsigned Error = lodepng_decode32_file(&Image, &Width, &Height, FilePath);
    if (Error)
    {
        sprintf(ErrorString, "Error decode file \"%s\": %s", FilePath, lodepng_error_text(Error)); return false;
    }
    
    PixelCount = Width * Height;
    Pixels = new PixelStruct[PixelCount];

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

    delete[] Image;
    Image = nullptr;

    char Char[MAX_CHAR_INT];
    sprintf(Char, "%d%d", Width, Height); //��������� ������ � ������ � ������
    InitRandom(atoi(Char)); //�������������� ������
    memset(Char, 0, sizeof(Char));
    
    for (int i = 0; i < 4; ++i) //�������� ����� ������������� ������� �������: ��������� ����� ������� ������ �����
    {
        char Temp[MAX_CHAR_INT];
        int SizeTemp = sprintf(Temp, "%lu", GetRandom()); //�������� ��������� ����� � ������
        Char[i] = Temp[SizeTemp - 1]; //����� ��������� ����� �� ������� � ���������� ���
    }

    InitRandom(atoi(Char)); //����� �������������� ������ ����� ������
    return true;
}
//-----------------------------------------------------------------------------
char* libCrypter::PrepareMessage(const char *Message)
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
    char *MessageComplete = new char[Size];
    MessageComplete[Size] = '\0';
    sprintf(MessageComplete, "%s%s", StringReserve, Message);
    return MessageComplete;
}
//-----------------------------------------------------------------------------
bool libCrypter::CheckMessage(const char *MessageComplete, size_t Size)
{
    if (strlen(MessageComplete) >= PixelCount) //���� ������ ��������� ������ ��� ����� ���������� �������� - ����������� ������� ��������� ��� ����� ���������
    {
        sprintf(ErrorString, "This image is too small for your message."); return false;
    }

    for (size_t i = 0; i < Size; ++i) //����� ��������� � �������� ������� ������� �� ����������
    {
        char Char = MessageComplete[i];
        if (Char < 32 && Char > 255)
        {
            sprintf(ErrorString, "Invalid char: %c. Ascii code: %d.", Char, Char); return false;
        }
    }

    //������� ������ ��� ���������� �������� ��������� �����
    VectorRandom = new unsigned long[Size];
    memset(VectorRandom, 0, Size);
    return true;
}
//-----------------------------------------------------------------------------
bool libCrypter::WriteFile(const char *PathOutput)
{
    unsigned char* Image = new unsigned char[PixelCount * 4];
    
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

    delete[] Image;
    Image = nullptr;

    if (Error)
    {
        sprintf(ErrorString, "Error encode file \"%s\": %s", PathOutput, lodepng_error_text(Error)); return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
void libCrypter::InitRandom(unsigned long Digit)
{
    Random = Digit;
}
//-----------------------------------------------------------------------------
unsigned long libCrypter::GetRandom()
{
    this->Random ^= (this->Random << 21);
    this->Random ^= (this->Random >> 35);
    this->Random ^= (this->Random << 4);
    return this->Random;
}
//-----------------------------------------------------------------------------
unsigned long libCrypter::GetRandom(unsigned long Minimum, unsigned long Maximum)
{
    return Minimum + GetRandom() % Maximum;
}
//-----------------------------------------------------------------------------
size_t libCrypter::GetSizeReserveString()
{
    //��������� ������ �������� � ������
    char PixelCountString[MAX_CHAR_INT];
    sprintf(PixelCountString, "%d", PixelCount);
    return strlen(PixelCountString);
}
//-----------------------------------------------------------------------------
bool libCrypter::ContainsVector(unsigned long Value, size_t MessageSize)
{
    bool Result = false;
    for (size_t i = 0; i < MessageSize; ++i)
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
bool libCrypter::FileExist(const char *FilePath)
{
    FILE *File = fopen(FilePath, "rb");
    bool Result = File ? true : false;
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
