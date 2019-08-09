#pragma once
#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED
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
    Bot(const QString &Token);
    ~Bot();

private:
    void NewMessage(const Telegram::Message &message);
    void StartProcess();
    void Process(const Telegram::Message &message);

private:
    Telegram::Bot *TelegramBot;
    QQueue<Telegram::Message> Queue;
    QFutureWatcher<void> *FutureWatcher;
    QSettings *Settings;
    QString UrlTemplate;
    QEventLoop *EventLoop;
    QNetworkAccessManager *NetworkAccessManager;
};
//-----------------------------------------------------------------------------
#endif
