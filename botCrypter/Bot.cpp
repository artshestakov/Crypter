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
Bot::Bot(const QString &token)
    : InstanceLib(NULL),
    Token(token),
    FutureWatcher(new QFutureWatcher<void>(this)),
    Settings(new QSettings(QCoreApplication::applicationDirPath() + "/Bot.ini", QSettings::IniFormat, this)),
    UrlTemplate("https://api.telegram.org/file/bot" + Token + "/"),
    NetworkAccessManager(new QNetworkAccessManager(this))
{
    
}
//-----------------------------------------------------------------------------
Bot::~Bot()
{
    FreeLibrary(InstanceLib);
}
//-----------------------------------------------------------------------------
bool Bot::InitCrypterLib()
{
    bool Result = true;
#ifdef WIN32
    QString Path = QCoreApplication::applicationDirPath() + "/libCrypter.dll";
    InstanceLib = LoadLibrary(Path.toStdWString().c_str());
    Result = InstanceLib != INVALID_HANDLE_VALUE ? true : false;
    if (Result)
    {
        crypt_message = (CryptMessage)GetProcAddress(InstanceLib, "CryptMessage");
        decrypt_message = (DecryptMessage)GetProcAddress(InstanceLib, "DecryptMessage");
        get_error = (GetError)GetProcAddress(InstanceLib, "GetError");
        Result = crypt_message && decrypt_message && get_error;
        if (!Result)
        {
            FreeLibrary(InstanceLib);
        }
    }
#endif
    return Result;
}
//-----------------------------------------------------------------------------
void Bot::Start()
{
    TelegramBot = new Telegram::Bot(Token, true, 500, 4, this);
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
        if (message.caption.isEmpty())
        {
            TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Сообщение с изображением не содержит текста. Пожалуйста, добавьте подпись, которая будет зашифрованна в отправляемое вами изображение."));
            return;
        }

        QString Url = CreateUrl(message);
        QEventLoop EventLoop;
        connect(NetworkAccessManager, &QNetworkAccessManager::finished, &EventLoop, &QEventLoop::quit);
        QNetworkReply *NetworkReply = NetworkAccessManager->get(QNetworkRequest(QUrl(Url)));
        NetworkReply->ignoreSslErrors();
 
        EventLoop.exec();

        if (NetworkReply->error() == QNetworkReply::NoError)
        {
            qDebug() << "Download is done. Writing...";
            QFile File(QCoreApplication::applicationDirPath() + "/" + QUuid::createUuid().toString() + "_" + QFileInfo(Url).fileName());
            if (File.open(QIODevice::WriteOnly))
            {
                QByteArray ByteArray = NetworkReply->readAll();
                if (File.write(ByteArray) == ByteArray.size())
                {
                    QString PathOutput = QCoreApplication::applicationDirPath() + "/" + QUuid::createUuid().toString() + ".png";
                    if (crypt_message(File.fileName().toStdString().c_str(), PathOutput.toStdString().c_str(), message.caption.toStdString().c_str()))
                    {
                        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Ваше сообщение \"%1\" было успешно зашифровано, ожидайте готовое изображение.").arg(message.caption));
                        TelegramBot->sendDocument(message.from.id, &QFile(PathOutput));
                    }
                    else
                    {
                        TelegramBot->sendMessage(message.from.id, get_error());
                    }
                }
                else
                {
                    TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("Произошла ошибка при сохранении файла на сервере."));
                }
                File.close();
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
QString Bot::CreateUrl(const Telegram::Message &message)
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
    return Url;
}
//-----------------------------------------------------------------------------
