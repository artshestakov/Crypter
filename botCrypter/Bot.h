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
class Bot : public QObject
{
    Q_OBJECT

public:
    Bot(const QString &token);
    ~Bot();

    void Start();

private:
    void NewMessage(const Telegram::Message &message);
    void StartProcess();
    void Process(const Telegram::Message &message);
   
    QString CreateUrl(const Telegram::Message &message);

private:
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
