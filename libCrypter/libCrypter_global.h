#pragma once
#ifndef _LIBCRYPTER_GLOBAL_H_INCLUDED
#define _LIBCRYPTER_GLOBAL_H_INCLUDED
//-----------------------------------------------------------------------------
#if defined(LIBCRYPTER_LIB)
#ifdef WIN32
# define LIBCRYPTER_EXPORT __declspec(dllexport)
#else
# define LIBCRYPTER_EXPORT __attribute__((visibility("default")))
#endif
#else
#ifdef WIN32
# define LIBCRYPTER_EXPORT __declspec(dllimport)
#else
# define LIBCRYPTER_EXPORT __attribute__((visibility("default")))
#endif
#endif
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

    /**
    *  одирование сообщени€ в изображение.
    * @param PathSource - путь к исходному изображению.
    * @param PathOutput - путь к изображению, в которое будет произведено кодирование сообщени€.
    * @param Message - константный указатель на строку, котора€ €вл€етс€ кодируемым сообщением.
    * @return возвращает 1 в случае успеха, иначе - 0.
    */
    LIBCRYPTER_EXPORT int CryptMessage(const char *PathSource, const char *PathOutput, const char *Message);

    /**
    * ƒекодирование сообщени€ из изображени€.
    * @param FilePath - путь к изображению, сообщение их которого необходимо получить.
    * @return возвращает констактный указатель на строку, в которой содержитс€ декодированное сообщение.
	* ¬Ќ»ћјЌ»≈! ¬ам необходимо самим позаботитьс€ об освобождении пам€ти возвращаемого значени€.
    */
    LIBCRYPTER_EXPORT const char* DecryptMessage(const char *FilePath);

    /**
    * ѕолучение текстового описани€ ошибки.
    * @return возвращает константный указатель на строку.
    */
    LIBCRYPTER_EXPORT const char* GetError(void);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif
