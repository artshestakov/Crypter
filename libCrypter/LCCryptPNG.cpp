#include "StdAfx.h"
#include "LCCryptPNG.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
LCCryptPNG::LCCryptPNG() 
	: LCAbstractCrypter(CrypterMode::PNG),
	Width(0),
	Height(0),
	PixelCount(0),
	Random(0)
{

}
//-----------------------------------------------------------------------------
LCCryptPNG::~LCCryptPNG()
{

}
//-----------------------------------------------------------------------------
bool LCCryptPNG::Crypt(const std::string &PathImage, const std::string &OutputPath, const std::string &SecretMessage)
{
    std::string Message = SecretMessage;
    bool Result = CheckFile(PathImage); //Проверка файла
    if (!Result)
    {
        return false;
    }

    Result = CheckPathOutput(OutputPath);
    if (!Result)
    {
        return false;
    }

    Result = ReadFile(PathImage); //Чтение изображения в память
    if (!Result)
    {
        return false;
    }

    Result = CheckMessage(Message); //Проверка сообщения
    if (!Result)
    {
        return false;
    }

    std::vector<size_t> VectorPoints = SearchIndexes(Message.size());
    for (size_t i = 0; i < VectorPoints.size(); i++) //Обход всех пикселей и изменение их альфа канала в соответствии с содержимым сообщения
    {
        LCColor Color = VectorPixels.at(VectorPoints.at(i));
        Color.Alpha = static_cast<unsigned char>(Message.at(i));
        VectorPixels[VectorPoints.at(i)] = Color;
    }

    std::vector<unsigned char> VectorImage;
    for (size_t i = 0; i < VectorPixels.size(); i++) //Обход всех пикселей и упаковка их данных в вектор
    {
        LCColor Color = VectorPixels.at(i);
        VectorImage.push_back(Color.Red);
        VectorImage.push_back(Color.Green);
        VectorImage.push_back(Color.Blue);
        VectorImage.push_back(Color.Alpha);
    }

    unsigned char *Image = reinterpret_cast<unsigned char*>(std::malloc(PixelCount * 4)); //Выделение памяти для шифрованного изображения
    if (Image == nullptr)
    {
        SetErrorString("Memory allocation is error.");
        return false;
    }

    for (size_t i = 0; i < VectorImage.size(); i++) //Обход памяти и заполнение её данными изображения
    {
        Image[i] = VectorImage.at(i);
    }

	unsigned int Error = lodepng_encode32_file(OutputPath.c_str(), Image, Width, Height); //Кодирование изображения в файл;
    if (Error) //Если кодирование произошло с ошибкой - выходим
    {
		SetErrorString(lodepng_error_text(Error));
        return false;
    }

    return Result;
}
//-----------------------------------------------------------------------------
bool LCCryptPNG::Decrypt(const std::string &PathImage, std::string &SecretMessage)
{
    bool Result = CheckFile(PathImage); //Проверка файла
    if (!Result)
    {
        return false;
    }

    Result = ReadFile(PathImage); //Чтение файла в память
    if (!Result)
    {
        return false;
    }

    int MessageSize = 0;
    bool SearchSize = true; //Флаг поиска размера сообщения

    while (true)
    {
        unsigned long RandomIndex = GetRandom(0, PixelCount);
        LCColor Color = VectorPixels.at(RandomIndex);
        if (Color.Alpha < 32 || Color.Alpha > 126)
        {
			SetErrorString("Invalid symbol.");
            Result = false;
            break;
        }

        SecretMessage.push_back(static_cast<char>(Color.Alpha));
        if (SearchSize) //Если идет поиск размера сообщения
        {
            if (SecretMessage.size() == std::to_string(PixelCount).size())
            {
                MessageSize = std::atoi(SecretMessage.c_str());
                SecretMessage.clear();
                SearchSize = false;
            }
        }
        else //Поиск сообщения
        {
            MessageSize--;
            if (!MessageSize) //Если формирование сообщения закончено
            {
                break;
            }
        }
    }

    return Result;
}
//-----------------------------------------------------------------------------
bool LCCryptPNG::CheckFile(const std::string &PathImage)
{
    if (PathImage.empty())
    {
		SetErrorString("Path to image is empty.");
        return false;
    }

    std::ifstream File;
    File.open(PathImage, std::ios_base::binary);
    if (File.fail())
    {
		SetErrorString("Image (" + PathImage + ") not open: " + strerror(errno));
        return false;
    }

    File.close();
    return true;
}
//-----------------------------------------------------------------------------
bool LCCryptPNG::CheckPathOutput(const std::string &PathOutput)
{
    std::ifstream File(PathOutput);
    if (File)
    {
        if (std::remove(PathOutput.c_str()))
        {
			SetErrorString("Not removed file: " + PathOutput);
            return false;
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
bool LCCryptPNG::CheckMessage(std::string &Message)
{
    if (Message.empty())
    {
		SetErrorString("Messages invalid.");
        return false;
    }

    std::deque<char> Deque;
    for (const char &Char : std::to_string(Message.size()))
    {
        Deque.push_back(Char);
    }

    while (true) //Формирование размера сообщения
    {
        if (Deque.size() == std::to_string(PixelCount).size())
        {
            break;
        }
        else
        {
            Deque.push_front('0');
        }
    }

    while (Deque.size())
    {
        Message.insert(0, 1, Deque.back());
        Deque.pop_back();
    }

    if (Message.size() > PixelCount) //Если сообщение (с его размером) больше массива пикселей
    {
		SetErrorString("Message invalid.");
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
bool LCCryptPNG::ReadFile(const std::string &PathImage)
{
    bool Result = false;

	std::vector<unsigned char> Image;
	unsigned int Error = lodepng::decode(Image, Width, Height, PathImage.c_str());
	if (Error) //Ошибка декодирования
	{
		SetErrorString(lodepng_error_text(Error));
	}
	else
	{
		PixelCount = Width * Height; //Количество пикселей
		for (size_t i = 0; i < Image.size(); i++) //Обход изображения побайтно и преобразование байтов в пиксели
		{
			LCColor Color;

			Color.Red = Image.at(i);

			i++;
			Color.Green = Image.at(i);

			i++;
			Color.Blue = Image.at(i);

			i++;
			Color.Alpha = Image.at(i);

			VectorPixels.push_back(Color);
		}

		InitRandom(static_cast<unsigned long>(std::stol(std::to_string(Width) + std::to_string(Height))));
		Result = true;
	}

    return Result;
}
//-----------------------------------------------------------------------------
std::vector<size_t> LCCryptPNG::SearchIndexes(size_t MessageSize)
{
    std::vector<size_t> VectorPoints;
    for (size_t i = 0; i < MessageSize; i++)
    {
        VectorPoints.push_back(GetRandom(0, PixelCount));
    }
    return VectorPoints;
}
//-----------------------------------------------------------------------------
void LCCryptPNG::InitRandom(unsigned long InitDigit)
{
    Random = InitDigit;
}
//-----------------------------------------------------------------------------
unsigned long LCCryptPNG::GetRandom()
{
    //Random ^= (Random << 21);
    //Random ^= (Random >> 35);
    //Random ^= (Random << 4);
    return this->Random;
}
//-----------------------------------------------------------------------------
unsigned long LCCryptPNG::GetRandom(unsigned long Min, unsigned long Max)
{
    return Min + GetRandom() % Max;
}
//-----------------------------------------------------------------------------
