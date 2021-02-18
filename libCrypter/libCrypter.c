#include "StdAfx.h"
//-----------------------------------------------------------------------------
#include "libCrypter.h"
#include "lodepng.h"
//-----------------------------------------------------------------------------
#define MAX_CHAR_INT 17 //Максимальное количество байт для числа
#define BEGIN_RANDOM_SIZE 4
//-----------------------------------------------------------------------------
char ErrorString[1024];
unsigned int ImageWidth = 0;
unsigned int ImageHeight = 0;
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
	_Bool Result = ReadFileToMemory(PathSource); //Чтение файла
	if (Result) //Файл успешно прочитан
	{
		char *MessageComplete = PrepareMessage(Message); //Подготовка сообщения
		Result = MessageComplete ? true : false;
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
				sprintf(Temp, "%d%d", ImageWidth, ImageHeight); //Переводим ширину и высоту в строку
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

	ImageWidth = 0;
	ImageHeight = 0;
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
		sprintf(Temp, "%d%d", ImageWidth, ImageHeight); //Переводим ширину и высоту в строку
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

	ImageWidth = 0;
	ImageHeight = 0;
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
	if (!FilePath) //Путь к файлу не указан
	{
		sprintf(ErrorString, "File path is empty.");
		return false;
	}

	if (!FileExist(FilePath)) //Файл не существует
	{
		sprintf(ErrorString, "File \"%s\" not exist.", FilePath);
		return false;
	}

	unsigned char* Image = NULL;
	unsigned Error = lodepng_decode32_file(&Image, &ImageWidth, &ImageHeight, FilePath);
	if (Error)
	{
		sprintf(ErrorString, "Error decode file \"%s\": %s", FilePath, lodepng_error_text(Error));
		return false;
	}
	PixelCount = ImageWidth * ImageHeight;
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
	//Запоминаем размер сообщения
	char MessageSize[MAX_CHAR_INT];
	sprintf(MessageSize, "%zu", strlen(Message));

	//Создаем строку, которая будет содержать размер сообщения
	size_t Size = GetSizeReserveString(); //Запоминаем размер строки с числом пикселей
	char StringReserve[MAX_CHAR_INT]; //Выделяем память равную размеру строки с пикселями
	StringReserve[Size] = '\0'; //Обрезаем лишний мусор
	memset(StringReserve, '0', Size); //Заполняем строку символами нуля (40 является кодом нуля)

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
_Bool CheckMessage(const char *MessageComplete, size_t Size)
{
	if (strlen(MessageComplete) >= PixelCount) //Если размер сообщения больше или равен количеству пикселей - изображение слишком маленькое для этого сообщения
	{
		sprintf(ErrorString, "This image is too small for your message.");
		return false;
	}

	for (size_t i = 0; i < Size; ++i) //Обход сообщения и проверка каждого символа на валидность
	{
        int Char = MessageComplete[i];
		if (Char < 32 && Char > 255)
		{
            sprintf(ErrorString, "Invalid char: %c. Ascii code: %d.", (char)Char, Char);
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

	unsigned int Error = lodepng_encode32_file(PathOutput, Image, ImageWidth, ImageHeight);

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
#ifdef WIN32
	HCRYPTPROV CryptoProvider = 0;
	bool Result = CryptAcquireContext(&CryptoProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT) == TRUE;
	if (Result) //Контекст создан успешно
	{
		Result = CryptGenRandom(CryptoProvider, sizeof(long long int), (PBYTE)(&Random)) == TRUE;
		if (Result) //Генерация прошла успешно - освобождаем контекст
		{
            if (CryptReleaseContext(CryptoProvider, 0) == FALSE) //Не удалось освободить контекст
			{
				printf("Error CryptReleaseContext()\n");
			}
		}
		else //Ошибка генерации
		{
			printf("Error CryptGenRandom()\n");
		}
	}
	else //Не удалось создать контекст
	{
		printf("Error CryptAcquireContext()\n");
	}
#else //Формирование соли под Linux
	FILE *FileDevice = fopen("/dev/random", "r");
	bool Result = FileDevice ? true : false;
	if (Result) //Устройство удалось открыть - читаем и закрываем устройство
	{
        Result = fread(&Random, sizeof(char), sizeof(Random), FileDevice) == sizeof(Random);
        fclose(FileDevice);
	}
	else
	{
        printf("Error read from /dev/random: %s\n", strerror(errno));
	}
#endif
    rand_t ReturnValue = Result ? Minimum + Random % Maximum : 0;
	if (ReturnValue < 0)
	{
		ReturnValue = -ReturnValue;
	}
	return ReturnValue;
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
	FILE *File = fopen(FilePath, "rb"); //Пытаемся открыть файл
	_Bool Result = File ? true : false;
	if (File) //Файл открылся
	{
		Result = fclose(File) == 0 ? true : false;
		if (!Result)
		{
			sprintf(ErrorString, "Error close file at checking exist: %s.", strerror(errno));
		}
	}
	else //Ошибка открытия файла
	{
		Result = errno == ENOENT ? false : true;
	}
	return Result;
}
//-----------------------------------------------------------------------------
