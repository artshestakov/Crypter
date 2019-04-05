#pragma once
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
struct SNColor
{
    SNColor() : Red(0), Green(0), Blue(0), Alpha(0) { }
    SNColor(const unsigned char &red, const unsigned char &green, const unsigned char &blue, const unsigned char &alpha) : Red(red), Green(green), Blue(blue), Alpha(alpha) { }

    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
    unsigned char Alpha;
};
//-----------------------------------------------------------------------------
class SNImageCrypt
{
public:
    SNImageCrypt();
    ~SNImageCrypt();

    /** @brief Шифрование сообщений в файл */
    bool Crypt(const std::string &PathImage, const std::string &OutputPath, const std::string &SecretMessage);

    /** @brief Дешифрование сообщения. */
    bool Decrypt(const std::string &PathImage, std::string &SecretMessage);

    /** @brief Получить текст с ошибкой. */
    std::string GetErrorString() const;

private:

    /** @brief Инициализация переменных. */
    void ReInitSpace();

    /** @brief Проверка файла на возможность шифрования/дешифрования. */
    bool CheckFile(const std::string &PathImage);

    /** @brief Проверка выходного пути. */
    bool CheckPathOutput(const std::string &PathOutput);

    /** @brief Проверка сообщений на возможность их шифрования. */
    bool CheckMessage(std::string &Message);

    /** @brief Чтение изображения в память и разложение его на пиксели. */
    bool ReadFile(const std::string &PathImage);

    /** @brief Поиск случайных индексов одномерного массива-изображения. */
    std::vector<size_t> SearchIndexes(size_t MessageSize);

    /** @brief Начальная инициализация генератора псевдослучайных чисел. */
    void InitRandom(unsigned long InitDigit);

    /** @brief Сгенерировать псевдослучайное число. */
    unsigned long GetRandom();

    /** @brief Сгенерировать псевдослучайное число в диапазоне от Min до Max. */
    unsigned long GetRandom(unsigned long Min, unsigned long Max);

private:
    std::string ErrorString; //Строка с описанием ошибки
    unsigned int Width; //Ширина изображения
    unsigned int Height; //Высота изображения
    unsigned long PixelCount; //Количество пикселей в изображении

    std::vector<SNColor> VectorPixels; //Массив пикселей
    unsigned long Random;
};
//-----------------------------------------------------------------------------
