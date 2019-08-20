#include "StdAfx.h"
#include "BCForceReply.h"
//-----------------------------------------------------------------------------
BCForceReply::BCForceReply(bool selective)
    : BCGenericReply(selective),
    ForceReply(true)
{

}
//-----------------------------------------------------------------------------
BCForceReply::~BCForceReply()
{

}
//-----------------------------------------------------------------------------
QString BCForceReply::Serialize() const
{
    QJsonObject JsonObject;
    JsonObject.insert("force_reply", ForceReply);
    JsonObject.insert("selective", Selective);
    return SerializeJson(JsonObject);
}
//-----------------------------------------------------------------------------
