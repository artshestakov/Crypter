#pragma once
#ifndef _GCTHREAD_H_INCLUDED
#define _GCTHREAD_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------------------
class GCThread : public QThread
{
    Q_OBJECT

signals:
    void Crypted(bool Result, const QString &ErrorString, const QString &PathOutput);
    void Decrypted(bool Result, const QString &ErrorString, const QString &Message);

public:
    enum CryptMode
    {
        CM_Unknown,
        CM_Crypt,
        CM_Decrypt,
    };

public:
    GCThread(QObject *parent = 0);
    virtual ~GCThread();

    void Crypt(const QString &path_source, const QString &path_output, const QString &message);
    void Decrypt(const QString &path_source);

protected:
    void run() override;
    void ResultSignal();

private:
    QString ErrorString;
    bool LastResult;
    CryptMode CurrentMode;
    QString PathSource;
    QString PathOutput;
    QString Message;
};
//-----------------------------------------------------------------------------
#endif
