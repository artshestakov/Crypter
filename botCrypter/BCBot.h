#pragma once
#ifndef _BOT_H_INCLUDED
#define _BOT_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "QTelegramBot.h"
#include "Types/Reply/BCReplyKeyboardMarkup.h"
//-----------------------------------------------------------------------------
class BCBot : public QObject
{
    Q_OBJECT

public:
    BCBot(const QString &token);
    ~BCBot();

    void Start();

private:
    void NewMessage(const BCTypeMessage &message);
    bool DownloadFile(const BCTypeMessage &message, QString &FilePath);
    void ShowDebugString(const QString &String);

private:
    QString Token;
    QTelegramBot *TelegramBot;
    QFutureWatcher<void> *FutureWatcher;
    QSettings *Settings;
    QString UrlTemplate;
    QNetworkAccessManager *NetworkAccessManager;
};
//-----------------------------------------------------------------------------
#endif
