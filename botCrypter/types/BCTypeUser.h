#pragma once
#ifndef _BCTYPEUSER_H_INCLUDED
#define _BCTYPEUSER_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeUser
{
public:
    BCTypeUser();
    BCTypeUser(QJsonObject JsonObject);
    ~BCTypeUser();

    qint32 ID;
    QString FirstName;
    QString LastName;
    QString UserName;
};
//-----------------------------------------------------------------------------
#endif
