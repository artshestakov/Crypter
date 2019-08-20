#pragma once
#ifndef _BCTYPECALLBACKQUERY_H_INCLUDED
#define _BCTYPECALLBACKQUERY_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeCallbackQuery
{
public:
    BCTypeCallbackQuery();
    BCTypeCallbackQuery(QJsonObject JsonObject);
    ~BCTypeCallbackQuery();

    bool isEmpty() const;

    QString ID;
    BCTypeUser From;
    BCTypeMessage Message;
    QString InlineMessageID;
    QString ChatInstance;
    QString Data;
    QString GameShortName;

private:
    bool Empty;
};
//-----------------------------------------------------------------------------
#endif
