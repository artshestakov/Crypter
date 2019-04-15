#pragma once
//-----------------------------------------------------------------------------

/**����������� ����� � �����������.
\param PathImage ���� � ��������� �����������
\param PathResult ���� � ����������� � ������������� �������
\param Message �����
*/
extern "C" __declspec(dllexport) int Crypt(const char *PathImage, const char *PathResult, const char *Message);

/**������������ ����� �� �����������.
\param PathImage ���� � �����������, ����������� ������������� �����
\return ���������� �����, ������������� � �����������.
*/
extern "C" __declspec(dllexport) const char* Decrypt(const char *PathImage);

/**�������� �������� ������.
\return ���������� �������� ������.
*/
extern "C" __declspec(dllexport) const char* GetErrorString();
//-----------------------------------------------------------------------------
