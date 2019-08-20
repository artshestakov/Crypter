#pragma once
#ifndef _BCTYPEUPDATE_H_INCLUDED
#define _BCTYPEUPDATE_H_INCLUDED
//-----------------------------------------------------------------------------
#include "BCTypeMessage.h"
#include "BCTypeCallbackQuery.h"
//-----------------------------------------------------------------------------
class BCTypeUpdate
{
public:
    BCTypeUpdate();
    BCTypeUpdate(QJsonObject JsonObject);
    ~BCTypeUpdate();

    quint32 ID;
    BCTypeMessage Message;
    BCTypeCallbackQuery CallbackQuery;
};
//-----------------------------------------------------------------------------
#endif
