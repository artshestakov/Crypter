#pragma once
#ifndef _CONVERTER_H_INCLUDED
#define _CONVERTER_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------------------
class BCConverter
{
public:
    BCConverter(const QString &file_path);
    ~BCConverter();

    bool Convert(QString &ErrorString);
    QString GetFilePath() const;

private:
    QString FilePath;
};
//-----------------------------------------------------------------------------
#endif
