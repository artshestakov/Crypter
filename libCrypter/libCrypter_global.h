#pragma once
#ifndef _LIBCRYPTER_GLOBAL_H_INCLUDED
#define _LIBCRYPTER_GLOBAL_H_INCLUDED
//-----------------------------------------------------------------------------
#ifdef LIBCRYPTER_LIB
# define LIBCRYPTER_EXPORT __declspec(dllexport)
#else
# define LIBCRYPTER_EXPORT __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * Кодирование сообщения в изображение.
    * @param PathSource - путь к исходному изображению.
    * @param PathOutput - путь к изображению, в которое будет произведено кодирование сообщения.
    * @param Message - константный указатель на строку, которая является кодируемым сообщением.
    * @returns возвращает 1 в случае успеха, иначе - 0.
    */
    LIBCRYPTER_EXPORT int CryptMessage(const char *PathSource, const char *PathOutput, const char *Message);

    /**
    * Декодирование сообщения из изображения.
    * @param FilePath - путь к изображению, сообщение их которого необходимо получить.
    * @return возвращает констактный указатель на строку, в которой содержится декодированное сообщение.
    */
    LIBCRYPTER_EXPORT const char* DecryptMessage(const char *FilePath);

    /**
    * Получение текстового описания ошибки.
    * @return возвращает константный указатель на строку.
    */
    LIBCRYPTER_EXPORT const char* GetError(void);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif
