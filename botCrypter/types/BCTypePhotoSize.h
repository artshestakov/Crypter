#pragma once
#ifndef _BCTYPEPHOTOSIZE_H_INCLUDED
#define _BCTYPEPHOTOSIZE_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypePhotoSize
{
public:
    BCTypePhotoSize();
    BCTypePhotoSize(QJsonObject JsonObject);
    ~BCTypePhotoSize();

    QString FileID;
    quint16 Width;
    quint16 Height;
    quint64 FileSize;
};
//-----------------------------------------------------------------------------
#endif
