#pragma once
#ifndef _BCREPLYKEYBOARDMARKUP_H_INCLUDED
#define _BCREPLYKEYBOARDMARKUP_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
class BCReplyKeyboardMarkup : public BCGenericReply
{
public:
    BCReplyKeyboardMarkup(QList<QStringList> keyboard, bool resize_keyboard = false, bool one_time_keyboard = false, bool selective = false);
    ~BCReplyKeyboardMarkup();

    QList<QStringList> Keyboard;
    bool ResizeKeyboard;
    bool OneTimeKeyboard;

    virtual QString Serialize() const override;
};
//-----------------------------------------------------------------------------
#endif
