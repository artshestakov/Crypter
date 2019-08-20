#pragma once
#ifndef _BCINLINEKEYBOARDMARKUP_H_INCLUDED
#define _BCINLINEKEYBOARDMARKUP_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
class BCInlineKeyboardButton
{
public:
    BCInlineKeyboardButton(const QString &text, const QString &url = QString(), const QString &callback_data = QString(), const QString &switch_inline_query = QString(), const QString &switch_inline_query_current_chat = QString());
    ~BCInlineKeyboardButton();

    QString Text;
    QString Url;
    QString CallbackData;
    QString SwitchInlineQuery;
    QString SwitchInlineQueryCurrentChat;

    QJsonObject ToJsonObject();

private:
    QJsonObject JsonObject;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class BCInlineKeyboardMarkup : public BCGenericReply
{
public:
    BCInlineKeyboardMarkup(QList<BCInlineKeyboardButton> buttons);
    ~BCInlineKeyboardMarkup();

    QList<BCInlineKeyboardButton> Buttons;

    virtual QString Serialize() const override;
};
//-----------------------------------------------------------------------------
#endif
