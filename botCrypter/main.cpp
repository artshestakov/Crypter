#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QEventLoop>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>
#include <QtCore/QQueue>
#include <QtCore/QFutureWatcher>
#include <QtCore/QFuture>
//-----------------------------------------------------------------------------
#include <QtConcurrent/QtConcurrent>
//-----------------------------------------------------------------------------
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
//-----------------------------------------------------------------------------
#include "qttelegrambot.h"
//-----------------------------------------------------------------------------
Telegram::Bot *TelegramBot = nullptr;
const QString TOKEN = "919167651:AAFWwPj2gRIyQ4xs2p7zWSA6noIzHxbaju8";
const QString FileUrl = "https://api.telegram.org/file/bot" + TOKEN + "/";
QQueue<Telegram::Message> Queue;
QFutureWatcher<void> *FutureWatcher = new QFutureWatcher<void>();
//-----------------------------------------------------------------------------
void NewMessage(const Telegram::Message &message);
void StartProcess();
void Process(const Telegram::Message &message);
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication Application(argc, argv);
    qDebug() << "Bot is starting...";
    TelegramBot = new Telegram::Bot(TOKEN, true, 500, 4);
    QObject::connect(TelegramBot, &Telegram::Bot::message, &NewMessage);
    qDebug() << "Bot is started";
    return Application.exec();
}
//-----------------------------------------------------------------------------
void NewMessage(const Telegram::Message &message)
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

    QFuture<void> Future = QtConcurrent::run(StartProcess);
    FutureWatcher->setFuture(Future);
}
//-----------------------------------------------------------------------------
void StartProcess()
{
    Process(Queue.dequeue());
}
//-----------------------------------------------------------------------------
void Process(const Telegram::Message &message)
{
    qDebug() << "Starting execute task";

    if (message.type == Telegram::Message::DocumentType || message.type == Telegram::Message::PhotoType)
    {
        QString Url;
        if (message.type == Telegram::Message::DocumentType)
        {
            qDebug() << "Document type.";
            Url = FileUrl + TelegramBot->getFile(message.document.fileId).filePath;
        }
        else if (message.type == Telegram::Message::PhotoType)
        {
            qDebug() << "Photo type.";
            Url = FileUrl + TelegramBot->getFile(message.photo.back().fileId).filePath;
        }
        
        QEventLoop EventLoop;
        QNetworkAccessManager NetworkAccessManager;

        QNetworkProxy NetworkProxy;
        NetworkProxy.setHostName("94.130.2.5");
        NetworkProxy.setPort(1080);
        NetworkProxy.setUser("proxyusr");
        NetworkProxy.setPassword("824691375");
        NetworkProxy.setType(QNetworkProxy::ProxyType::Socks5Proxy);
        NetworkAccessManager.setProxy(NetworkProxy);

        qDebug() << "Download file...";
        QNetworkReply *NetworkReply = NetworkAccessManager.get(QNetworkRequest(QUrl(Url)));

        QObject::connect(&NetworkAccessManager, &QNetworkAccessManager::finished, &EventLoop, &QEventLoop::quit);
        EventLoop.exec();

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
        qDebug() << "";
    }
    else
    {
        qDebug() << "Invalid message type.";
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Бот принимает только изображения."));
    }
}
//-----------------------------------------------------------------------------
