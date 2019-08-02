#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //������������ ���������� ���� ��� �����
#define SIZE_ERROR_STRING 1024 //������ ������ � ��������� ������
#define R_OK 1 //������������� ���������
#define R_ERROR 0 //������������� ���������
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
                for (int i = 0; i < MessageSize; ++i) //������� ��������� � ��������� ������ ������ � �������
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
    printf("DECRYPT: FilePath=%s\n", FilePath);
    char *Message = NULL;
    if (ReadFile(FilePath))
    {
        size_t Size = GetSizeReserveString();
        printf("DECRYPT: SizeReverseString=%d\n", Size);
        char *Buffer = (char *)malloc(Size);
        for (size_t i = 0; i < Size; ++i)
        {
            Buffer[i] = Pixels[GetRandom(1, PixelCount)].A;
        }

        int MessageSize = atoi(Buffer);
        printf("DECRYPT: MessageSize=%d\n", MessageSize);

        free(Buffer);
        Buffer = NULL;

        Message = (char *)malloc(MessageSize);
        for (int i = 0; i < MessageSize; ++i)
        {
            Message[i] = Pixels[GetRandom(1, PixelCount)].A;
        }
        Message[MessageSize] = '\0';
        printf("DECRYPT: Message=%s\n", Message);
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
    printf("GetSizeReserveString=%d\n", Size);
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
    printf("MessageComplete=%s\n", MessageComplete);
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

    //������� ������ ��� ���������� �������� ��������� �����
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
    //��������� ������ �������� � ������
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
