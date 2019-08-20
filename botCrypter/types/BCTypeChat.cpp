#include "StdAfx.h"
#include "BCTypeChat.h"
//-----------------------------------------------------------------------------
BCTypeChat::BCTypeChat()
{

}
//-----------------------------------------------------------------------------
BCTypeChat::BCTypeChat(QJsonObject JsonObject)
{
    ID = JsonObject.value("id").toInt();
    QString chatType = JsonObject.value("type").toString();
    
    if (chatType == "private")
    {
        Type = ChatType::Private;
    }
    else if (chatType == "group")
    {
        Type = ChatType::Group;
    }
    else if (chatType == "channel")
    {
        Type = ChatType::Channel;
    }

    UserName = JsonObject.value("username").toString();
    FirstName = JsonObject.value("first_name").toString();
    LastName = JsonObject.value("last_name").toString();
}
//-----------------------------------------------------------------------------
BCTypeChat::~BCTypeChat()
{

}
//-----------------------------------------------------------------------------
