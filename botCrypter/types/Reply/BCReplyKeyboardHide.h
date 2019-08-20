#pragma once
#ifndef _BCREPLYKEYBOARDHIDE_H_INCLUDED
#define _BCREPLYKEYBOARDHIDE_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
class BCReplyKeyboardHide : public BCGenericReply
{
public:
    BCReplyKeyboardHide(bool selective = false);
    ~BCReplyKeyboardHide();
    
    const bool HideKeyboard;

    virtual QString Serialize() const override;
};
//-----------------------------------------------------------------------------
#endif
