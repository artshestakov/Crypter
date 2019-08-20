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
    return SerializeJson(QJsonObject{ { "hide_keyboard", HideKeyboard }, { "selective", Selective } });
}
//-----------------------------------------------------------------------------
