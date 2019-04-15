#pragma once
//-----------------------------------------------------------------------------

/**Зашифровать текст в изображение.
\param PathImage Путь к исходному изображению
\param PathResult Путь к изображению с зашифрованным текстом
\param Message Текст
*/
extern "C" __declspec(dllexport) int Crypt(const char *PathImage, const char *PathResult, const char *Message);

/**Расшифровать текст из изображения.
\param PathImage Путь к изображению, содержащему зашифрованный текст
\return Возвращает текст, зашифрованный в изображение.
*/
extern "C" __declspec(dllexport) const char* Decrypt(const char *PathImage);

/**Получить описания ошибки.
\return Возвращает описание ошибки.
*/
extern "C" __declspec(dllexport) const char* GetErrorString();
//-----------------------------------------------------------------------------
