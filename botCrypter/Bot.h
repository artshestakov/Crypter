#pragma once
#ifndef _BOT_H_INCLUDED
#define _BOT_H_INCLUDED
//-----------------------------------------------------------------------------
#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QFutureWatcher>
#include <QtCore/QSettings>
//-----------------------------------------------------------------------------
#include "qttelegrambot.h"
//-----------------------------------------------------------------------------
#ifdef WIN32
#include <windows.h>
#endif
//-----------------------------------------------------------------------------
typedef int(__stdcall *CryptMessage)(const char *PathSource, const char *PathOutput, const char *Message);
typedef const char*(__stdcall *DecryptMessage)(const char *PathSource);
typedef const char*(__stdcall *GetError)(void);
//-----------------------------------------------------------------------------
class Bot : public QObject
{
    Q_OBJECT

public:
    Bot(const QString &token);
    ~Bot();

    bool InitCrypterLib();
    void Start();

private:
    void NewMessage(const Telegram::Message &message);
    void StartProcess();
    void Process(const Telegram::Message &message);
   
    QString CreateUrl(const Telegram::Message &message);

private:
    HINSTANCE InstanceLib;
    CryptMessage crypt_message;
    DecryptMessage decrypt_message;
    GetError get_error;

    QString Token;
    Telegram::Bot *TelegramBot;
    QQueue<Telegram::Message> Queue;
    QFutureWatcher<void> *FutureWatcher;
    QSettings *Settings;
    QString UrlTemplate;
    QNetworkAccessManager *NetworkAccessManager;
};
//-----------------------------------------------------------------------------
#endif
