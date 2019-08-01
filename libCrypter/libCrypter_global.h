#pragma once
#ifndef _LIBCRYPTER_GLOBAL_H_INCLUDED
#define _LIBCRYPTER_GLOBAL_H_INCLUDED
//-----------------------------------------------------------------------------
#ifdef LIBCRYPTER_EXPORTS
# define SLCORE_EXPORT __declspec(dllexport)
#else
# define SLCORE_EXPORT __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
extern "C" LIBCRYPTER_API void GetErrorString()
{
    
}
//-----------------------------------------------------------------------------
#endif
