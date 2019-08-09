#include <QtCore/QDebug>
#include <QtCore/QEventLoop>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>
#include <QtCore/QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include "Bot.h"
//-----------------------------------------------------------------------------
Bot::Bot(const QString &Token)
    : TelegramBot(new Telegram::Bot(Token, true, 500, 4, this)),
    FutureWatcher(new QFutureWatcher<void>(this)),
    Settings(new QSettings(QCoreApplication::applicationDirPath() + "/Bot.ini", QSettings::IniFormat, this)),
    UrlTemplate("https://api.telegram.org/file/bot" + Token + "/"),
    EventLoop(new QEventLoop(this)),
    NetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(TelegramBot, &Telegram::Bot::message, this, &Bot::NewMessage);

    if (Settings->value("Proxy/Use").toBool())
    {
        QNetworkProxy NetworkProxy(QNetworkProxy::Socks5Proxy);
        NetworkProxy.setHostName(Settings->value("Proxy/Host").toString());
        NetworkProxy.setPort(Settings->value("Proxy/Port").toInt());
        NetworkProxy.setUser(Settings->value("Proxy/User").toString());
        NetworkProxy.setPassword(Settings->value("Proxy/Password").toString());
        NetworkAccessManager->setProxy(NetworkProxy);
    }
}
//-----------------------------------------------------------------------------
Bot::~Bot()
{

}
//-----------------------------------------------------------------------------
void Bot::NewMessage(const Telegram::Message &message)
{
    Queue.enqueue(message);
    if (Queue.size() == 1 && !FutureWatcher->isRunning())
    {
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Очередь свободна. Ваше изображение будет обработано сразу."));
    }
    else
    {
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Вы %1 в очереди. Пожалуйста, подождите...").arg(Queue.size()));
    }

    QFuture<void> Future = QtConcurrent::run(this, &Bot::StartProcess);
    FutureWatcher->setFuture(Future);
}
//-----------------------------------------------------------------------------
void Bot::StartProcess()
{
    Process(Queue.dequeue());
}
//-----------------------------------------------------------------------------
void Bot::Process(const Telegram::Message &message)
{
    if (message.type == Telegram::Message::DocumentType || message.type == Telegram::Message::PhotoType)
    {
        QString Url;
        switch (message.type)
        {
        case Telegram::Message::DocumentType:
            Url = UrlTemplate + TelegramBot->getFile(message.document.fileId).filePath;
            break;

        case Telegram::Message::PhotoType:
            Url = UrlTemplate + TelegramBot->getFile(message.photo.back().fileId).filePath;
            break;
        }

        QNetworkReply *NetworkReply = NetworkAccessManager->get(QNetworkRequest(QUrl(Url)));
        NetworkReply->ignoreSslErrors();

        connect(NetworkAccessManager, &QNetworkAccessManager::finished, EventLoop, &QEventLoop::quit);
        EventLoop->exec();

        if (NetworkReply->error() == QNetworkReply::NoError)
        {
            qDebug() << "Download is done. Writing...";
            QFile File(QCoreApplication::applicationDirPath() + "/" + QUuid::createUuid().toString() + "_" + QFileInfo(Url).fileName());
            if (File.open(QIODevice::WriteOnly))
            {
                File.write(NetworkReply->readAll());
                File.close();
                TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Ваше изображение успешно сохранено на сервере."));
            }
            else
            {
                qDebug() << "Error writing:" << File.errorString();
            }
        }
        else
        {
            qDebug() << "Download is error:" << NetworkReply->errorString();
        }
        delete NetworkReply;
    }
    else
    {
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Бот принимает только изображения (фотография или документ)."));
    }
}
//-----------------------------------------------------------------------------
