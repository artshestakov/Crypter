#pragma once
#ifndef _BCTYPESTICKER_H_INCLUDED
#define _BCTYPESTICKER_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeSticker
{
public:
    BCTypeSticker();
    BCTypeSticker(QJsonObject JsonObject);
    ~BCTypeSticker();

    QString FileID;
    quint16 Width;
    quint16 Height;
    BCTypePhotoSize PhotoSize;
    quint64 FileSize;
};
//-----------------------------------------------------------------------------
#endif
