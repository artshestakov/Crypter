#pragma once
#ifndef _BCTYPEVIDEO_H_INCLUDED
#define _BCTYPEVIDEO_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCTypePhotoSize.h"
//-----------------------------------------------------------------------------
class BCTypeVideo
{
public:
    BCTypeVideo();
    BCTypeVideo(QJsonObject JsonObject);
    ~BCTypeVideo();

    QString FileID;
    quint16 Width;
    quint16 Height;
    quint64 Duration;
    BCTypePhotoSize PhotoSize;
    QString MimeType;
    QString FileSize;
};
//-----------------------------------------------------------------------------
#endif
