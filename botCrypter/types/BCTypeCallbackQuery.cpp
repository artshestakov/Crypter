#include "StdAfx.h"
#include "BCTypeUser.h"
#include "BCTypeMessage.h"
#include "BCTypeCallbackQuery.h"
//-----------------------------------------------------------------------------
BCTypeCallbackQuery::BCTypeCallbackQuery()
    : Empty(true)
{

}
//-----------------------------------------------------------------------------
BCTypeCallbackQuery::BCTypeCallbackQuery(QJsonObject JsonObject) : Empty(false)
{
    ID = JsonObject.value("id").toString();
    From = BCTypeUser(JsonObject.value("from").toObject());
    
    if (JsonObject.contains("message"))
    {
        Message = BCTypeMessage(JsonObject.value("message").toObject());
    }
    
    if (JsonObject.contains("inline_message_id"))
    {
        InlineMessageID = JsonObject.value("inline_message_id").toString();
    }
    
    if (JsonObject.contains("chat_instance"))
    {
        ChatInstance = JsonObject.value("chat_instance").toString();
    }
    
    if (JsonObject.contains("data"))
    {
        Data = JsonObject.value("data").toString();
    }
    
    if (JsonObject.contains("game_short_name"))
    {
        GameShortName = JsonObject.value("game_short_name").toString();
    }
}
//-----------------------------------------------------------------------------
BCTypeCallbackQuery::~BCTypeCallbackQuery()
{

}
//-----------------------------------------------------------------------------
bool BCTypeCallbackQuery::isEmpty() const
{
    return Empty;
}
//-----------------------------------------------------------------------------
