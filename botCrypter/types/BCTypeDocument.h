#pragma once
#ifndef _BCTYPEDOCUMENT_H_INCLUDED
#define _BCTYPEDOCUMENT_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCTypePhotoSize.h"
//-----------------------------------------------------------------------------
class BCTypeDocument
{
public:
    BCTypeDocument();
    BCTypeDocument(QJsonObject JsonObject);
    ~BCTypeDocument();

    QString FileID;
    BCTypePhotoSize PhotoSize;
    QString FileName;
    QString MimeType;
    quint64 FileSize;
};
//-----------------------------------------------------------------------------
#endif
