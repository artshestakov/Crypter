#pragma once
#ifndef _BCTYPEMESSAGE_H_INCLUDED
#define _BCTYPEMESSAGE_H_INCLUDED
//-----------------------------------------------------------------------------
#include <QDateTime>
#include "BCTypeAudio.h"
#include "BCTypeDocument.h"
#include "BCTypePhotoSize.h"
#include "BCTypeSticker.h"
#include "BCTypeVideo.h"
#include "BCTypeVoice.h"
#include "BCTypeContact.h"
#include "BCTypeLocation.h"
#include "BCTypeChat.h"
#include "BCTypeUser.h"
//-----------------------------------------------------------------------------
class BCTypeMessage
{
public:
    BCTypeMessage();
    BCTypeMessage(QJsonObject JsonObject);
    ~BCTypeMessage();

    enum MessageType
    {
        TextType,
        AudioType,
        DocumentType,
        PhotoType,
        StickerType,
        VideoType,
        VoiceType,
        ContactType,
        LocationType,
        NewChatParticipantType,
        LeftChatParticipantType,
        NewChatTitleType,
        NewChatPhotoType,
        DeleteChatPhotoType,
        GroupChatCreatedType
    };

    quint32 ID;
    QDateTime Date;
    BCTypeChat Chat;

    BCTypeUser From;
    BCTypeUser ForwardFrom;
    QDateTime ForwardDate;
    BCTypeMessage *ReplyToMessage;

    MessageType Type;
    QString Caption;

    QString String;
    BCTypeUser User;
    BCTypeAudio Audio;
    BCTypeDocument Document;
    QList<BCTypePhotoSize> Photo;
    BCTypeSticker Sticker;
    BCTypeVideo Video;
    BCTypeVoice Voice;
    BCTypeContact Contact;
    BCTypeLocation Location;
    bool Boolean;
};
//-----------------------------------------------------------------------------
#endif
