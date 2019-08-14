#include "networking.h"

#include <ctime>
#include <QNetworkProxy>

using namespace Telegram;

Networking::Networking(QString token, QObject *parent)
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

Networking::~Networking()
{
    delete NetworkAccessManager;
}

QByteArray Networking::request(QString endpoint, ParameterList params, Networking::Method method)
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

    QNetworkRequest req;
    QUrl url = buildUrl(endpoint);
    req.setUrl(url);

    QEventLoop loop;

    QNetworkReply *reply;

    if (method == GET)
    {
        url.setQuery(parameterListToString(params));
        req.setUrl(url);
        reply = NetworkAccessManager->get(req);
    }
    else if (method == POST)
    {
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        reply = NetworkAccessManager->post(req, parameterListToString(params));
    }
    else if (method == UPLOAD)
    {
        QByteArray boundary = generateMultipartBoundary(params);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
        QByteArray requestData = generateMultipartFormData(params, boundary);
        req.setHeader(QNetworkRequest::ContentLengthHeader, requestData.length());
        reply = NetworkAccessManager->post(req, requestData);
    }
    else
    {
        qCritical("No valid method!");
        reply = NULL;
    }

    if (reply == NULL)
    {
        qWarning("Reply is NULL");
        delete reply;
        return QByteArray();
    }

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qCritical("%s", qPrintable(QString("[%1] %2").arg(reply->error()).arg(reply->errorString())));
        delete reply;
        return QByteArray();
    }

    QByteArray ret = reply->readAll();
    delete reply;
    return ret;
}

QUrl Networking::buildUrl(QString endpoint)
{
    QUrl url = QUrl();
    url.setScheme("https");
    url.setHost(API_HOST);
    url.setPath("/bot" + TokenString + endpoint);
    return url;
}

QByteArray Networking::parameterListToString(ParameterList list)
{
    QByteArray ret;
    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        ret.append(i.key() + "=" + i.value().value + "&");
        ++i;
    }
    ret = ret.left(ret.length() - 1);
    return ret;
}

QByteArray Networking::generateMultipartBoundary(ParameterList list)
{
    // Generates a boundary that is not existent in the data
    QByteArray result;

    srand((unsigned int)time(NULL));
    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        if (i.value().isFile)
        {
            while (result.isEmpty() || i.value().value.contains(result))
            {
                result.append(generateRandomString(4));
            }
        }
        ++i;
    }

    return result;
}

QByteArray Networking::generateMultipartFormData(ParameterList list, QByteArray boundary)
{
    QByteArray result;

    ParameterList::iterator i = list.begin();
    while (i != list.end())
    {
        HttpParameter param = i.value();
        result.append("--" + boundary + "\r\n");
        result.append("Content-Disposition: form-data; name=\"" + i.key());
        if (param.isFile)
        {
            result.append("\"; filename=\"" + param.filename);
        }
        result.append("\"\r\n");
        if (param.isFile)
        {
            result.append("Content-Type: " + param.mimeType + "\r\n");
        }
        result.append("\r\n");
        result.append(param.value);
        result.append("\r\n");

        ++i;
    }
    result.append("--" + boundary + "--");
    return result;
}

QString Networking::generateRandomString(int length)
{
    static const std::string chars("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890-=[]\\;',./!@#$%^&*()_+{}|:\"<>?`~");
    static const size_t charsLen = chars.length();
    QString result;
    for (int i = 0; i < length; ++i) {
        result += QChar(chars[rand() % charsLen]);
    }
    return result;
}
