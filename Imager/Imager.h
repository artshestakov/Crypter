#pragma once
#ifndef IMAGER_H_INCLUDED
#define IMAGER_H_INCLUDED
//-----------------------------------------------------------------------------
struct PixelStruct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
//-----------------------------------------------------------------------------
class Imager
{
public:
    Imager();
    ~Imager();

    const char* GetErrorString() const;
    bool Crypt(const char *PathSource, const char *PathOutput, const char *Message);
    const char* Decrypt(const char *FilePath);

private:
    bool ReadFile(const char *FilePath);
    char* PrepareMessage(const char *Message);
    bool CheckMessage(const char *MessageComplete, size_t Size);
    bool WriteFile(const char *PathOutput);
    
    void InitRandom(unsigned long Digit);
    unsigned long GetRandom();
    unsigned long GetRandom(unsigned long Minimum, unsigned long Maximum);
    size_t GetSizeReserveString();
    bool ContainsVector(unsigned long Value, size_t MessageSize);
    bool FileExist(const char *FilePath);

private:
    char *ErrorString;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelCount;
    PixelStruct *Pixels;
    unsigned long *VectorRandom;
    unsigned long Random;
};
//-----------------------------------------------------------------------------
#endif
