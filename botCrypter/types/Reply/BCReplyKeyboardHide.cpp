#include "StdAfx.h"
#include "BCReplyKeyboardHide.h"
//-----------------------------------------------------------------------------
BCReplyKeyboardHide::BCReplyKeyboardHide(bool selective)
    : BCGenericReply(selective),
    HideKeyboard(true)
{

}
//-----------------------------------------------------------------------------
BCReplyKeyboardHide::~BCReplyKeyboardHide()
{

}
//-----------------------------------------------------------------------------
QString BCReplyKeyboardHide::Serialize() const
{
    QJsonObject JsonObject;
    JsonObject.insert("hide_keyboard", HideKeyboard);
    JsonObject.insert("selective", Selective);
    return SerializeJson(JsonObject);
}
//-----------------------------------------------------------------------------
