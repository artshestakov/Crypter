#pragma once
//-----------------------------------------------------------------------------
#include "LCPixel.h"
//-----------------------------------------------------------------------------
class LCCrypter
{
public:
    LCCrypter();
    ~LCCrypter();

    //Шифрование сообщения в файл
    bool Crypt(const std::string &PathImage, const std::string &OutputPath, const std::string &SecretMessage);

    //Дешифрование сообщения
    bool Decrypt(const std::string &PathImage, std::string &SecretMessage);

	//Получение описания ошибки
	std::string GetErrorString() const;

private:

    //Проверка файла на возможность шифрования/дешифрования
    bool CheckFile(const std::string &PathImage);

    //Проверка выходного пути
    bool CheckPathOutput(const std::string &PathOutput);

    //Проверка сообщений на возможность их шифрования
    bool CheckMessage(std::string &Message);

    //Чтение изображения в память и разложение его на пиксели
    bool ReadFile(const std::string &PathImage);

    //Поиск случайных индексов одномерного массива-изображения
	std::vector<unsigned long> SearchIndexes(size_t MessageSize);

    //Начальная инициализация генератора псевдослучайных чисел
    void InitRandom(unsigned long InitDigit);

    //Сгенерировать псевдослучайное число
    unsigned long GetRandom();

    //Сгенерировать псевдослучайное число в диапазоне от Min до Max
    unsigned long GetRandom(unsigned long Min, unsigned long Max);

private:
	std::string ErrorString; //Текстовое описание ошибки
    unsigned int Width; //Ширина изображения
    unsigned int Height; //Высота изображения
    unsigned long PixelCount; //Количество пикселей в изображении

    std::vector<LCPixel> VectorPixels; //Массив пикселей
    unsigned long Random; //Рандомное число
};
//-----------------------------------------------------------------------------
