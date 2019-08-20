#include "BCBot.h"
#include "BCConverter.h"
#include "libCrypter_global.h"
//-----------------------------------------------------------------------------
BCBot::BCBot(const QString &token)
    : Token(token),
    FutureWatcher(new QFutureWatcher<void>(this)),
    Settings(new QSettings(QCoreApplication::applicationDirPath() + "/Bot.ini", QSettings::IniFormat, this)),
    UrlTemplate("https://api.telegram.org/file/bot" + Token + "/"),
    NetworkAccessManager(new QNetworkAccessManager(this))
{
    
}
//-----------------------------------------------------------------------------
BCBot::~BCBot()
{
    
}
//-----------------------------------------------------------------------------
void BCBot::Start()
{
    TelegramBot = new Telegram::Bot(Token, true, 500, 4, this);
    connect(TelegramBot, &Telegram::Bot::message, this, &BCBot::NewMessage);

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
void BCBot::NewMessage(const Telegram::Message &message)
{
    ShowDebugString(QString());
    ShowDebugString(QString("Incoming message %1 from %2").arg(message.id).arg(message.from.firstname + " " + message.from.lastname + " (" + message.from.username + ")"));

    bool Result = message.type == Telegram::Message::DocumentType || message.type == Telegram::Message::PhotoType;
    if (!Result) //�������� ��� ���������
    {
        ShowDebugString(QString("Invalid message type: %1").arg(message.type));
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("��� ��������� ������ ����������� (����������� ������������ � ���� ��������� �����������)."));
        return;
    }

    if (message.caption.isEmpty()) //������� � ����������� �����������
    {
        ShowDebugString("Caption image is null");
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("��������� � ������������ �� �������� ������. ����������, �������� �������, ������� ����� ������������� � ������������ ���� �����������."));
        return;
    }

    QString FilePath;
    QString ErrorString;
    Result = DownloadFile(message, FilePath);
    if (!Result) //������ ���������� �����
    {
        return;
    }

    QString Suffix = QFileInfo(FilePath).suffix().toLower();
    if (Suffix != "png") //���� ���������� ����� �������� �� PNG - ������������ � PNG
    {
        ShowDebugString(QString("Converting image (%1) to png format...").arg(Suffix));
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("����������� ������ ����������� � ������ PNG..."));

        BCConverter converter(FilePath);
        Result = converter.Convert(ErrorString);
        if (Result)
        {
            ShowDebugString("Converting is done");
            TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("����������� ������� ���������."));
            FilePath = converter.GetFilePath();
        }
        else
        {
            ShowDebugString("Converting failed: " + ErrorString);
            TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("����������� ����������� � �������: ") + ErrorString);
            return;
        }
    }

    ShowDebugString("Encoding...");
    TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("������� ������� �����������..."));

    QString PathOutput = QCoreApplication::applicationDirPath() + "/" + QUuid::createUuid().toString() + ".png";
    Result = CryptMessage(FilePath.toStdString().c_str(), PathOutput.toStdString().c_str(), message.caption.toStdString().c_str()) == 1;
    if (Result)
    {
        ShowDebugString("Encoding is done. Sending result image...");
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("����������� ������� ���������, �������� ������� �����������."));
        Result = TelegramBot->sendDocument(message.from.id, &QFile(PathOutput));
        if (Result)
        {
            ShowDebugString("Sending result image - done");
        }
        else
        {
            ShowDebugString("Sending result image - failed");
        }
    }
    else
    {
        ErrorString = GetError();
        ShowDebugString("Encoding failed: " + ErrorString);
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("����������� ����������� � �������: ") + ErrorString);
    }
}
//-----------------------------------------------------------------------------
bool BCBot::DownloadFile(const Telegram::Message &message, QString &FilePath)
{
    QString Url;
    switch (message.type)
    {
    case Telegram::Message::DocumentType: Url = UrlTemplate + TelegramBot->getFile(message.document.fileId).filePath; break;
    case Telegram::Message::PhotoType: Url = UrlTemplate + TelegramBot->getFile(message.photo.back().fileId).filePath; break;
    }

    ShowDebugString("Downloading image...");
    TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("��������� �����������..."));

    QEventLoop EventLoop;
    connect(NetworkAccessManager, &QNetworkAccessManager::finished, &EventLoop, &QEventLoop::quit);
    QNetworkReply *NetworkReply = NetworkAccessManager->get(QNetworkRequest(QUrl(Url)));
    NetworkReply->ignoreSslErrors();
    EventLoop.exec();

    QByteArray ByteArray;
    QString ErrorString;
    
    bool Result = NetworkReply->error() == QNetworkReply::NoError;
    Result ? ByteArray = NetworkReply->readAll() : ErrorString = NetworkReply->errorString();

    if (Result)
    {
        ShowDebugString("Image downloaded. Writing...");
        QFile File(QCoreApplication::applicationDirPath() + "/" + QUuid::createUuid().toString().replace("{", QString()).replace("}", QString()) + "_" + QFileInfo(Url).fileName());
        Result = File.open(QIODevice::WriteOnly);
        if (Result) //������ �������� ����� ��� ������
        {
            Result = File.write(ByteArray) == ByteArray.size();
            if (Result) //������ ��� ������ � ����
            {
                FilePath = File.fileName();
                ShowDebugString("Writed - done");
            }
            else
            {
                ShowDebugString("Writed - failed: " + File.errorString());
                TelegramBot->sendMessage(message.from.id, "Error write ByteArray to File: " + File.errorString());
            }
            File.close();
        }
        else
        {
            TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("��������� ������ ��� ���������� ����� �� �������: %1").arg(File.errorString()));
        }
    }
    else
    {
        ShowDebugString("Error download: " + ErrorString);
        TelegramBot->sendMessage(message.from.id, QString::fromLocal8Bit("�� ������� ������� ���� � �������� Telegram: %1").arg(ErrorString));
    }
    
    delete NetworkReply;
    return Result;
}
//-----------------------------------------------------------------------------
void BCBot::ShowDebugString(const QString &String)
{
    if (String.isEmpty())
    {
        qDebug().noquote() << QString();
    }
    else
    {
        qDebug().noquote() << QString("%1 %2").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(String);
    }
}
//-----------------------------------------------------------------------------
