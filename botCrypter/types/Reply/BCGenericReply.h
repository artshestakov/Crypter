#pragma once
#ifndef _BCGENERICREPLY_H_INCLUDED
#define _BCGENERICREPLY_H_INCLUDED
//-----------------------------------------------------------------------------
class BCGenericReply
{
public:
    BCGenericReply();
    BCGenericReply(bool selective);
    ~BCGenericReply();

    bool Selective;

    virtual QString Serialize() const;
    bool IsValid() const;

protected:
    QByteArray SerializeJson(QJsonObject JsonObject) const;

private:
    bool Valid;
};
//-----------------------------------------------------------------------------
#endif
