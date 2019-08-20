#include "StdAfx.h"
#include "BCTypeContact.h"
//-----------------------------------------------------------------------------
BCTypeContact::BCTypeContact()
{

}
//-----------------------------------------------------------------------------
BCTypeContact::BCTypeContact(QJsonObject JsonObject)
{
    PhoneNumber = JsonObject.value("phone_number").toString();
    FirstName = JsonObject.value("first_name").toString();
    LastName = JsonObject.value("last_name").toString();
    UserID = JsonObject.value("user_id").toInt();
}
//-----------------------------------------------------------------------------
BCTypeContact::~BCTypeContact()
{

}
//-----------------------------------------------------------------------------
