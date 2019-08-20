#include "StdAfx.h"
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
BCGenericReply::BCGenericReply()
    : Valid(false)
{

}
//-----------------------------------------------------------------------------
BCGenericReply::BCGenericReply(bool selective)
    : Selective(selective),
    Valid(true)
{

}
//-----------------------------------------------------------------------------
BCGenericReply::~BCGenericReply()
{

}
//-----------------------------------------------------------------------------
QString BCGenericReply::Serialize() const
{
    return QString();
}
//-----------------------------------------------------------------------------
bool BCGenericReply::IsValid() const
{
    return Valid;
}
//-----------------------------------------------------------------------------
QByteArray BCGenericReply::SerializeJson(QJsonObject JsonObject) const
{
    return QJsonDocument(JsonObject).toJson(QJsonDocument::Compact);
}
//-----------------------------------------------------------------------------
