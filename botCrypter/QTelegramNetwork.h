#pragma once
#ifndef _QTELEGRAMNETWORK_H_INCLUDED
#define _QTELEGRAMNETWORK_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------------------
#define API_HOST                                    "api.telegram.org"
#define ENDPOINT_GET_ME                             "/getMe"
#define ENDPOINT_SEND_MESSAGE                       "/sendMessage"
#define ENDPOINT_FORWARD_MESSAGE                    "/forwardMessage"
#define ENDPOINT_SEND_PHOTO                         "/sendPhoto"
#define ENDPOINT_SEND_AUDIO                         "/sendAudio"
#define ENDPOINT_SEND_DOCUMENT                      "/sendDocument"
#define ENDPOINT_SEND_STICKER                       "/sendSticker"
#define ENDPOINT_SEND_VIDEO                         "/sendVideo"
#define ENDPOINT_SEND_VOICE                         "/sendVoice"
#define ENDPOINT_SEND_LOCATION                      "/sendLocation"
#define ENDPOINT_SEND_CHAT_ACTION                   "/sendChatAction"
#define ENDPOINT_ANSWER_CALLBACK_QUERY              "/answerCallbackQuery"
#define ENDPOINT_GET_USER_PROFILE_PHOTOS            "/getUserProfilePhotos"
#define ENDPOINT_GET_UPDATES                        "/getUpdates"
#define ENDPOINT_SET_WEBHOOK                        "/setWebhook"
#define ENDPOINT_GET_FILE                           "/getFile"
//-----------------------------------------------------------------------------
class QHttpParameter
{
public:
    QHttpParameter();
    QHttpParameter(const QVariant &value, bool is_file = false, const QString &mime_type = "text/plain", const QString & file_name = QString());
    ~QHttpParameter();

    QByteArray Value;
    bool IsFile;
    QString MimeType;
    QString FileName;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef QMap<QString, QHttpParameter> ParameterList;
//-----------------------------------------------------------------------------
class QTelegramNetwork : public QObject
{
    Q_OBJECT

public:
    enum QueryType
    {
        GET,
        POST,
        UPLOAD
    };

    QTelegramNetwork(const QString &token, QObject *parent = 0);
    ~QTelegramNetwork();

    QByteArray Request(const QString &endpoint, const ParameterList &params, QTelegramNetwork::QueryType method);

private:
    QNetworkAccessManager *NetworkAccessManager;
    QString TokenString;

    QByteArray ParameterListToString(ParameterList list);

    QByteArray GenerateMultipartBoundary(ParameterList list);
    QByteArray generateMultipartFormData(ParameterList list, const QByteArray &boundary);
    QString GenerateRandomString(int Length);
};
//-----------------------------------------------------------------------------
#endif
