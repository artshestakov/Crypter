#pragma once
#ifndef _BCTYPECONTACT_H_INCLUDED
#define _BCTYPECONTACT_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeContact
{
public:
    BCTypeContact();
    BCTypeContact(QJsonObject JsonObject);
    ~BCTypeContact();

    QString PhoneNumber;
    QString FirstName;
    QString LastName;
    quint32 UserID;
}; 
//-----------------------------------------------------------------------------
#endif
