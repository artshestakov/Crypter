#include "StdAfx.h"
#include "BCTypeUser.h"
//-----------------------------------------------------------------------------
BCTypeUser::BCTypeUser()
    : ID(0)
{

}
//-----------------------------------------------------------------------------
BCTypeUser::BCTypeUser(QJsonObject JsonObject)
{
    ID = JsonObject.value("id").toInt();
    FirstName = JsonObject.value("first_name").toString();
    LastName = JsonObject.value("last_name").toString();
    UserName = JsonObject.value("username").toString();
}
//-----------------------------------------------------------------------------
BCTypeUser::~BCTypeUser()
{

}
//-----------------------------------------------------------------------------
