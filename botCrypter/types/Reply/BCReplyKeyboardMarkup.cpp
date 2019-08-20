#include "StdAfx.h"
#include "BCReplyKeyboardMarkup.h"
//-----------------------------------------------------------------------------
BCReplyKeyboardMarkup::BCReplyKeyboardMarkup(QList<QStringList> keyboard, bool resize_keyboard, bool one_time_keyboard, bool selective)
    : BCGenericReply(selective),
    Keyboard(keyboard),
    ResizeKeyboard(resize_keyboard),
    OneTimeKeyboard(one_time_keyboard)
{

}
//-----------------------------------------------------------------------------
BCReplyKeyboardMarkup::~BCReplyKeyboardMarkup()
{

}
//-----------------------------------------------------------------------------
QString BCReplyKeyboardMarkup::Serialize() const
{
    QJsonObject JsonObject;
    QJsonArray KeyboardMarkup;
    
    foreach(QStringList StringList, Keyboard)
    {
        KeyboardMarkup.append(QJsonArray::fromStringList(StringList));
    }

    JsonObject.insert("keyboard", KeyboardMarkup);
    JsonObject.insert("resize_keyboard", ResizeKeyboard);
    JsonObject.insert("one_time_keyboard", OneTimeKeyboard);
    JsonObject.insert("selective", Selective);
    return SerializeJson(JsonObject);
}
//-----------------------------------------------------------------------------
