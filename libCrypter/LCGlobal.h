#pragma once
//-----------------------------------------------------------------------------
extern "C" __declspec(dllexport) int Crypt(const char *PathImage, const char *PathResult, const char *Message);
extern "C" __declspec(dllexport) const char* Decrypt(const char *PathImage);
extern "C" __declspec(dllexport) const char* GetErrorString();
//-----------------------------------------------------------------------------
