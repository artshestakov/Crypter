#pragma once
#ifndef _BCFORCEREPLY_H_INCLUDED
#define _BCFORCEREPLY_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCGenericReply.h"
//-----------------------------------------------------------------------------
class BCForceReply : public BCGenericReply
{
public:
    BCForceReply(bool selective = false);
    ~BCForceReply();

    const bool ForceReply;

    virtual QString Serialize() const override;
};
//-----------------------------------------------------------------------------
#endif
