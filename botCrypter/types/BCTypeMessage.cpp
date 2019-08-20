#include "StdAfx.h"
#include "BCTypeMessage.h"
//-----------------------------------------------------------------------------
BCTypeMessage::BCTypeMessage()
{

}
//-----------------------------------------------------------------------------
BCTypeMessage::BCTypeMessage(QJsonObject JsonObject)
{
    ID = JsonObject.value("message_id").toInt();
    Date = QDateTime::fromMSecsSinceEpoch(JsonObject.value("date").toInt());
    Chat = BCTypeChat(JsonObject.value("chat").toObject());

    if (JsonObject.contains("from"))
    {
        From = BCTypeUser(JsonObject.value("from").toObject());
    }

    if (JsonObject.contains("forward_from"))
    {
        ForwardFrom = BCTypeUser(JsonObject.value("forward_from").toObject());
    }
    
    if (JsonObject.contains("forward_date"))
    {
        ForwardDate = QDateTime::fromMSecsSinceEpoch(JsonObject.value("forward_date").toInt());
    }
    
    if (JsonObject.contains("reply_to_message"))
    {
        ReplyToMessage = new BCTypeMessage(JsonObject.value("reply_to_message").toObject());
    }

    QJsonObject Object;
    if (JsonObject.contains("text"))
    {
        String = JsonObject.value("text").toString();
        Type = BCTypeMessage::TextType;
    }
    
    if (JsonObject.contains("audio"))
    {
        Object = JsonObject.value("audio").toObject();
        Audio = BCTypeAudio(Object);
        Type = BCTypeMessage::AudioType;
    }

    if (JsonObject.contains("document"))
    {
        Object = JsonObject.value("document").toObject();
        Document = BCTypeDocument(Object);
        Caption = JsonObject.contains("caption") ? JsonObject.value("caption").toString() : QString();
        Type = BCTypeMessage::DocumentType;
    }

    if (JsonObject.contains("photo"))
    {
        foreach (QJsonValue JsonValue, JsonObject.value("photo").toArray())
        {
            Photo.append(BCTypePhotoSize(JsonValue.toObject()));
        }
        Caption = JsonObject.contains("caption") ? JsonObject.value("caption").toString() : QString();
        Type = BCTypeMessage::PhotoType;
    }

    if (JsonObject.contains("sticker"))
    {
        Object = JsonObject.value("sticker").toObject();
        Sticker = BCTypeSticker(Object);
        Type = BCTypeMessage::StickerType;
    }

    if (JsonObject.contains("video"))
    {
        Object = JsonObject.value("video").toObject();
        Video = BCTypeVideo(Object);
        Caption = JsonObject.contains("caption") ? JsonObject.value("caption").toString() : QString();
        Type = BCTypeMessage::VideoType;
    }
    
    if (JsonObject.contains("voice"))
    {
        Object = JsonObject.value("voice").toObject();
        Voice = BCTypeVoice(Object);
        Type = BCTypeMessage::VoiceType;
    }

    if (JsonObject.contains("contact"))
    {
        Object = JsonObject.value("contact").toObject();
        Contact = BCTypeContact(Object);
        Type = BCTypeMessage::ContactType;
    }

    if (JsonObject.contains("location"))
    {
        Object = JsonObject.value("location").toObject();
        Location = BCTypeLocation(Object);
        Type = BCTypeMessage::LocationType;
    }
    
    if (JsonObject.contains("new_chat_participant"))
    {
        Object = JsonObject.value("new_chat_participant").toObject();
        User = BCTypeUser(Object);
        Type = BCTypeMessage::NewChatParticipantType;
    }

    if (JsonObject.contains("left_chat_participant"))
    {
        Object = JsonObject.value("left_chat_participant").toObject();
        User = BCTypeUser(Object);
        Type = BCTypeMessage::LeftChatParticipantType;
    }

    if (JsonObject.contains("new_chat_title"))
    {
        String = JsonObject.value("new_chat_title").toString();
        Type = BCTypeMessage::NewChatTitleType;
    }
    
    if (JsonObject.contains("new_chat_photo"))
    {
        foreach (QJsonValue val, JsonObject.value("new_chat_photo").toArray())
        {
            Photo.append(BCTypePhotoSize(val.toObject()));
        }
        Type = BCTypeMessage::NewChatPhotoType;
    }

    if (JsonObject.contains("delete_chat_photo"))
    {
        Boolean = true;
        Type = BCTypeMessage::DeleteChatPhotoType;
    }
    
    if (JsonObject.contains("group_chat_created"))
    {
        Boolean = true;
        Type = BCTypeMessage::GroupChatCreatedType;
    }
}
//-----------------------------------------------------------------------------
BCTypeMessage::~BCTypeMessage()
{

}
//-----------------------------------------------------------------------------
