#include "StdAfx.h"
#include "BCInlineKeyboardMarkup.h"
//-----------------------------------------------------------------------------
BCInlineKeyboardButton::BCInlineKeyboardButton(const QString &text, const QString &url, const QString &callback_data, const QString &switch_inline_query, const QString &switch_inline_query_current_chat)
    : Text(text),
    Url(url),
    CallbackData(callback_data),
    SwitchInlineQuery(switch_inline_query),
    SwitchInlineQueryCurrentChat(switch_inline_query_current_chat)
{
    JsonObject.insert("text", text);
    
    if (!url.isEmpty())
    {
        JsonObject.insert("url", url);
        return;
    }
    
    if (!callback_data.isEmpty())
    {
        JsonObject.insert("callback_data", callback_data);
    }
    
    if (!switch_inline_query.isEmpty())
    {
        JsonObject.insert("switch_inline_query", switch_inline_query);
    }
    
    if (!switch_inline_query_current_chat.isEmpty())
    {
        JsonObject.insert("switch_inline_query_current_chat", switch_inline_query_current_chat);
    }
}
//-----------------------------------------------------------------------------
BCInlineKeyboardButton::~BCInlineKeyboardButton()
{

}
//-----------------------------------------------------------------------------
QJsonObject BCInlineKeyboardButton::ToJsonObject()
{
    return JsonObject;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BCInlineKeyboardMarkup::BCInlineKeyboardMarkup(QList<BCInlineKeyboardButton> buttons)
    : BCGenericReply(false),
    Buttons(buttons)
{
    qDebug() << "InlineKeyboardMarkup Constructor";
}
//-----------------------------------------------------------------------------
BCInlineKeyboardMarkup::~BCInlineKeyboardMarkup()
{

}
//-----------------------------------------------------------------------------
QString BCInlineKeyboardMarkup::Serialize() const
{
    QJsonObject JsonObject;
    QJsonArray KeyboardButtons;

    foreach(BCInlineKeyboardButton InlineKeyboardButton, Buttons)
    {
        QJsonArray JsonArray;
        JsonArray.append(InlineKeyboardButton.ToJsonObject());
        KeyboardButtons.append(JsonArray);
    }
    
    JsonObject.insert("inline_keyboard", KeyboardButtons);
    qDebug() << JsonObject;
    return SerializeJson(JsonObject);
}
//-----------------------------------------------------------------------------
