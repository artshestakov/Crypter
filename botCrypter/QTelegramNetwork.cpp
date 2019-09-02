#include "QTelegramNetwork.h"
//-----------------------------------------------------------------------------
QHttpParameter::QHttpParameter()
{

}
//-----------------------------------------------------------------------------
QHttpParameter::QHttpParameter(const QVariant &value, bool is_file, const QString &mime_type, const QString & file_name)
    : Value(value.toByteArray()),
    IsFile(is_file),
    MimeType(mime_type),
    FileName(file_name)
{

}
//-----------------------------------------------------------------------------
QHttpParameter::~QHttpParameter()
{

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QTelegramNetwork::QTelegramNetwork(const QString &token, QObject *parent)
    : QObject(parent),
    NetworkAccessManager(new QNetworkAccessManager(this)),
    TokenString(token)
{
    QNetworkProxy NetworkProxy;
    NetworkProxy.setHostName("94.130.2.5");
    NetworkProxy.setPort(1080);
    NetworkProxy.setUser("proxyusr");
    NetworkProxy.setPassword("824691375");
    NetworkProxy.setType(QNetworkProxy::ProxyType::Socks5Proxy);
    NetworkAccessManager->setProxy(NetworkProxy);
}
//-----------------------------------------------------------------------------
QTelegramNetwork::~QTelegramNetwork()
{
    
}
//-----------------------------------------------------------------------------
QByteArray QTelegramNetwork::Request(const QString &endpoint, const ParameterList &params, QTelegramNetwork::QueryType query_type)
{
    if (endpoint.isEmpty())
    {
        qWarning("Cannot do request without endpoint");
        return QByteArray();
    }

    if (TokenString.isEmpty())
    {
        qWarning("Cannot do request without a Telegram Bot Token");
        return QByteArray();
    }

    QNetworkRequest NetworkRequest;

    QUrl Url;
    Url.setScheme("https");
    Url.setHost(API_HOST);
    Url.setPath("/bot" + TokenString + endpoint);
    NetworkRequest.setUrl(Url);

    QEventLoop EventLoop;
    QNetworkReply *NetworkReply;

    if (query_type == QTelegramNetwork::GET)
    {
        Url.setQuery(ParameterListToString(params));
        NetworkRequest.setUrl(Url);
        NetworkReply = NetworkAccessManager->get(NetworkRequest);
    }
    else if (query_type == QTelegramNetwork::POST)
    {
        NetworkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        NetworkReply = NetworkAccessManager->post(NetworkRequest, ParameterListToString(params));
    }
    else if (query_type == QTelegramNetwork::UPLOAD)
    {
        QByteArray boundary = GenerateMultipartBoundary(params);
        NetworkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
        QByteArray requestData = generateMultipartFormData(params, boundary);
        NetworkRequest.setHeader(QNetworkRequest::ContentLengthHeader, requestData.length());
        NetworkReply = NetworkAccessManager->post(NetworkRequest, requestData);
    }
    else
    {
        qCritical("No valid method!");
        NetworkReply = NULL;
    }

    if (!NetworkReply)
    {
        qWarning("NetworkReply is NULL");
        delete NetworkReply;
        NetworkReply = nullptr;
        return QByteArray();
    }

    QObject::connect(NetworkReply, &QNetworkReply::finished, &EventLoop, &QEventLoop::quit);
    EventLoop.exec();

    if (NetworkReply->error() != QNetworkReply::NoError)
    {
        qCritical("%s", qPrintable(QString("[%1] %2").arg(NetworkReply->error()).arg(NetworkReply->errorString())));
        delete NetworkReply;
        NetworkReply = nullptr;
        return QByteArray();
    }

    QByteArray Result = NetworkReply->readAll();
    delete NetworkReply;
    NetworkReply = nullptr;
    return Result;
}
//-----------------------------------------------------------------------------
QByteArray QTelegramNetwork::ParameterListToString(ParameterList list)
{
    QByteArray Result;
    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        Result.append(i.key() + "=" + i.value().Value + "&");
        ++i;
    }
    Result = Result.left(Result.length() - 1);
    return Result;
}
//-----------------------------------------------------------------------------
QByteArray QTelegramNetwork::GenerateMultipartBoundary(ParameterList list)
{
    QByteArray Result;
    
    srand(static_cast<unsigned int>(time(NULL)));
    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        if (i.value().IsFile)
        {
            while (Result.isEmpty() || i.value().Value.contains(Result))
            {
                Result.append(GenerateRandomString(4));
            }
        }
        ++i;
    }

    return Result;
}
//-----------------------------------------------------------------------------
QByteArray QTelegramNetwork::generateMultipartFormData(ParameterList list, const QByteArray &boundary)
{
    QByteArray Result;

    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        QHttpParameter param = i.value();
        Result.append("--" + boundary + "\r\n");
        Result.append("Content-Disposition: form-data; name=\"" + i.key());
        if (param.IsFile)
        {
            Result.append("\"; filename=\"" + param.FileName);
        }
        Result.append("\"\r\n");
        if (param.IsFile)
        {
            Result.append("Content-Type: " + param.MimeType + "\r\n");
        }
        Result.append("\r\n");
        Result.append(param.Value);
        Result.append("\r\n");

        ++i;
    }
    Result.append("--" + boundary + "--");
    return Result;
}
//-----------------------------------------------------------------------------
QString QTelegramNetwork::GenerateRandomString(int Length)
{
    static const std::string Chars("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890-=[]\\;',./!@#$%^&*()_+{}|:\"<>?`~");
    static const size_t CharsLength = Chars.length();
    QString Result;
    for (int i = 0; i < Length; ++i)
    {
        Result += QChar(Chars[rand() % CharsLength]);
    }
    return Result;
}
//-----------------------------------------------------------------------------
