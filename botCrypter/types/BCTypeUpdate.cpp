#include "StdAfx.h"
#include "BCTypeUpdate.h"
//-----------------------------------------------------------------------------
BCTypeUpdate::BCTypeUpdate()
{

}
//-----------------------------------------------------------------------------
BCTypeUpdate::BCTypeUpdate(QJsonObject JsonObject)
{
    ID = JsonObject.value("update_id").toInt();
    Message = BCTypeMessage(JsonObject.value("message").toObject());
    if (JsonObject.contains("callback_query"))
    {
        CallbackQuery = BCTypeCallbackQuery(JsonObject.value("callback_query").toObject());
    }
}
//-----------------------------------------------------------------------------
BCTypeUpdate::~BCTypeUpdate()
{

}
//-----------------------------------------------------------------------------
