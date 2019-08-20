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
    return SerializeJson(QJsonObject{ { "force_reply", ForceReply }, { "selective", Selective } });
}
//-----------------------------------------------------------------------------
