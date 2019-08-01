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
extern "C" LIBCRYPTER_EXPORT int Crypt(const char *PathSource, const char *PathOutput, const char *Message);
extern "C" LIBCRYPTER_EXPORT const char* Decrypt(const char *FilePath);
//-----------------------------------------------------------------------------
#endif
