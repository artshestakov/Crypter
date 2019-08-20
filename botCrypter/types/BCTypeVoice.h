#pragma once
#ifndef _BCTYPEVOICE_H_INCLUDED
#define _BCTYPEVOICE_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeVoice
{
public:
    BCTypeVoice();
    BCTypeVoice(QJsonObject JsonObject);
    ~BCTypeVoice();

    QString FileID;
    quint64 Duration;
    QString MimeType;
    quint64 FileSize;
};
//-----------------------------------------------------------------------------
#endif
