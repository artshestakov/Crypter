#pragma once
#ifndef _LIBCRYPTER_GLOBAL_H_INCLUDED
#define _LIBCRYPTER_GLOBAL_H_INCLUDED
//-----------------------------------------------------------------------------
#if defined(LIBCRYPTER_LIB)
# define LIBCRYPTER_EXPORT __attribute__((visibility("default")))//__declspec(dllexport)
#else
# define LIBCRYPTER_EXPORT __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * ����������� ��������� � �����������.
    * @param PathSource - ���� � ��������� �����������.
    * @param PathOutput - ���� � �����������, � ������� ����� ����������� ����������� ���������.
    * @param Message - ����������� ��������� �� ������, ������� �������� ���������� ����������.
    * @return ���������� 1 � ������ ������, ����� - 0.
    */
    LIBCRYPTER_EXPORT int CryptMessage(const char *PathSource, const char *PathOutput, const char *Message);

    /**
    * ������������� ��������� �� �����������.
    * @param FilePath - ���� � �����������, ��������� �� �������� ���������� ��������.
    * @return ���������� ����������� ��������� �� ������, � ������� ���������� �������������� ���������.
	* ��������! ��� ���������� ����� ������������ �� ������������ ������ ������������� ��������.
    */
    LIBCRYPTER_EXPORT const char* DecryptMessage(const char *FilePath);

    /**
    * ��������� ���������� �������� ������.
    * @return ���������� ����������� ��������� �� ������.
    */
    LIBCRYPTER_EXPORT const char* GetError(void);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif
