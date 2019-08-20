#pragma once
#ifndef _BCTYPECHAT_H_INCLUDED
#define _BCTYPECHAT_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeChat
{
public:
    BCTypeChat();
    BCTypeChat(QJsonObject JsonObject);
    ~BCTypeChat();

    enum ChatType
    {
        Private,
        Group,
        Channel
    };

    qint32 ID;
    ChatType Type;
    QString Title;
    QString UserName;
    QString FirstName;
    QString LastName;
};
//-----------------------------------------------------------------------------
#endif
