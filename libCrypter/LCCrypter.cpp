#include "StdAfx.h"
#include "LCCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
LCCrypter::LCCrypter() :
	ErrorString("No error."),
	Width(0),
	Height(0),
	PixelCount(0),
	Random(0)
{

}
//-----------------------------------------------------------------------------
LCCrypter::~LCCrypter()
{
	
}
//-----------------------------------------------------------------------------
bool LCCrypter::Crypt(const std::string &PathImage, const std::string &OutputPath, const std::string &SecretMessage)
{
    std::string Message = SecretMessage;
    bool Result = CheckFile(PathImage); //Проверка файла
    if (!Result)
    {
        return false;
    }

    Result = CheckPathOutput(OutputPath); //Проверка пути для записи результата
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

	std::vector<unsigned long> VectorPoints = SearchIndexes(Message.size());
    for (size_t i = 0; i < VectorPoints.size(); i++) //Обход всех пикселей и изменение их альфа канала в соответствии с содержимым сообщения
    {
        LCPixel Color = VectorPixels.at(VectorPoints.at(i));
        Color.Alpha = static_cast<unsigned char>(Message.at(i));
        VectorPixels[VectorPoints.at(i)] = Color;
    }

    std::vector<unsigned char> VectorImage;
    for (size_t i = 0; i < VectorPixels.size(); i++) //Обход всех пикселей и упаковка их данных в вектор
    {
        LCPixel Color = VectorPixels.at(i);
        VectorImage.push_back(Color.Red);
        VectorImage.push_back(Color.Green);
        VectorImage.push_back(Color.Blue);
        VectorImage.push_back(Color.Alpha);
    }

    unsigned char *Image = reinterpret_cast<unsigned char*>(std::malloc(PixelCount * 4)); //Выделение памяти для шифрованного изображения
    if (Image == nullptr)
    {
        ErrorString = "Memory allocation is error.";
        return false;
    }

    for (size_t i = 0; i < VectorImage.size(); i++) //Обход памяти и заполнение её данными изображения
    {
        Image[i] = VectorImage.at(i);
    }

	unsigned int Error = lodepng_encode32_file(OutputPath.c_str(), Image, Width, Height); //Кодирование изображения в файл;
    if (Error) //Если кодирование произошло с ошибкой - выходим
    {
		ErrorString = lodepng_error_text(Error);
        return false;
    }

    return Result;
}
//-----------------------------------------------------------------------------
bool LCCrypter::Decrypt(const std::string &PathImage, std::string &SecretMessage)
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
	std::vector<unsigned long> TempVector;

    while (true)
    {
        unsigned long RandomIndex = GetRandom(0, PixelCount);
		if (std::find(TempVector.begin(), TempVector.end(), RandomIndex) == TempVector.end())
		{
			TempVector.push_back(RandomIndex);
		}
		else
		{
			continue;
		}

        LCPixel Color = VectorPixels.at(RandomIndex);
		if (Color.Alpha == 127) //Если попался символ разделитель (размера сообщения и фактического сообщения)
		{
			MessageSize = std::atoi(SecretMessage.c_str());
			SecretMessage.clear();
			SearchSize = false;
			continue;
		}
		else //Разделитель ещё не нашли - продолжаем собирать размер сообщения
		{
			SecretMessage.push_back(static_cast<char>(Color.Alpha));
		}

        if (!SearchSize) //Если идет поиск размера сообщения
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
std::string LCCrypter::GetErrorString() const
{
	return ErrorString;
}
//-----------------------------------------------------------------------------
bool LCCrypter::CheckFile(const std::string &PathImage)
{
    if (PathImage.empty())
    {
		ErrorString = "Path to image is empty.";
        return false;
    }

    std::ifstream File;
    File.open(PathImage, std::ios_base::binary);
    if (File.fail())
    {
		ErrorString = "Image (" + PathImage + ") not open: " + strerror(errno);
        return false;
    }

    File.close();
    return true;
}
//-----------------------------------------------------------------------------
bool LCCrypter::CheckPathOutput(const std::string &PathOutput)
{
	const char *Path = PathOutput.c_str();
	if (std::ifstream(Path).good()) //Если файл существует - удаляем его
    {
        if (std::remove(Path) != 0) //Если файл не удалился
        {
			ErrorString = "Not removed file (" + PathOutput + "): " + strerror(errno);
            return false;
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
bool LCCrypter::CheckMessage(std::string &Message)
{
    if (Message.empty()) //Если сообщение пустое
    {
		ErrorString = "Message is empty.";
        return false;
    }

	std::string MessageSizeString = std::to_string(Message.size()); //Размер сообщения строкой
	MessageSizeString.push_back(SPLIT_SYMBOL); //Вставка символа-разделителя в конец размера сообщения
	std::reverse(MessageSizeString.begin(), MessageSizeString.end()); //Реверсируем строку с размером для удобства вставки в сообщение

    for (const char &Char : MessageSizeString) //Вставка размера в начало сообщения
    {
		Message.insert(0, 1, Char);
    }

    if (Message.size() > PixelCount) //Если сообщение (с его размером) больше массива пикселей
    {
		ErrorString = "Message is big for this image.";
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
bool LCCrypter::ReadFile(const std::string &PathImage)
{
    bool Result = false;

	std::vector<unsigned char> Image;
	unsigned int Error = lodepng::decode(Image, Width, Height, PathImage.c_str());
	if (Error) //Ошибка декодирования
	{
		ErrorString = lodepng_error_text(Error);
	}
	else
	{
		PixelCount = Width * Height; //Количество пикселей
		for (size_t i = 0; i < Image.size(); i++) //Обход изображения побайтно и преобразование байтов в пиксели
		{
			LCPixel Color;

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
std::vector<unsigned long> LCCrypter::SearchIndexes(size_t MessageSize)
{
	std::vector<unsigned long> VectorPoints;
	while (true)
	{
		unsigned long RandomIndex = GetRandom(0, PixelCount);
		if (std::find(VectorPoints.begin(), VectorPoints.end(), RandomIndex) == VectorPoints.end())
		{
			VectorPoints.push_back(RandomIndex);
		}

		if (VectorPoints.size() == MessageSize)
		{
			break;
		}
	}

    return VectorPoints;
}
//-----------------------------------------------------------------------------
void LCCrypter::InitRandom(unsigned long InitDigit)
{
    Random = InitDigit;
}
//-----------------------------------------------------------------------------
unsigned long LCCrypter::GetRandom()
{
    Random ^= (Random << 21);
    Random ^= (Random >> 35);
    Random ^= (Random << 4);
    return this->Random;
}
//-----------------------------------------------------------------------------
unsigned long LCCrypter::GetRandom(unsigned long Min, unsigned long Max)
{
    return Min + GetRandom() % Max;
}
//-----------------------------------------------------------------------------
