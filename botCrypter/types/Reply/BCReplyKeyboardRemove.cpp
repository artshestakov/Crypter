#include "StdAfx.h"
#include "BCReplyKeyboardRemove.h"
//-----------------------------------------------------------------------------
BCReplyKeyboardRemove::BCReplyKeyboardRemove(bool selective)
    : BCGenericReply(selective),
    RemoveKeyboard(true)
{

}
//-----------------------------------------------------------------------------
BCReplyKeyboardRemove::~BCReplyKeyboardRemove()
{

}
//-----------------------------------------------------------------------------
QString BCReplyKeyboardRemove::Serialize() const
{
    QJsonObject JsonObject;
    JsonObject.insert("remove_keyboard", RemoveKeyboard);
    if (Selective)
    {
        JsonObject.insert("selective", Selective);
    }
    qDebug() << JsonObject;
    return SerializeJson(JsonObject);
}
//-----------------------------------------------------------------------------
