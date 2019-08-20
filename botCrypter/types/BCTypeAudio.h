#pragma once
#ifndef _BCTYPEAUDIO_H_INCLUDED
#define _BCTYPEAUDIO_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeAudio
{
public:
    BCTypeAudio();
    BCTypeAudio(QJsonObject JsonObject);
    ~BCTypeAudio();

    QString FileID;
    quint64 Duration;
    QString Performer;
    QString Title;
    QString MimeType;
    quint64 FileSize;
};
//-----------------------------------------------------------------------------
#endif
