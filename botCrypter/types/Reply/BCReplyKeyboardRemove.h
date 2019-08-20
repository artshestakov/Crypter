#pragma once
#ifndef _BCREPLYKEYBOARDREMOVE_H_INCLUDED
#define _BCREPLYKEYBOARDREMOVE_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
class BCReplyKeyboardRemove : public BCGenericReply
{
public:
    BCReplyKeyboardRemove(bool selective = false);
    ~BCReplyKeyboardRemove();

    const bool RemoveKeyboard;

    virtual QString Serialize() const override;
};
//-----------------------------------------------------------------------------
#endif
