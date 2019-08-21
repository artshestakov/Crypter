#pragma once
#ifndef _QTELEGRAMBOT_H_INCLUDED
#define _QTELEGRAMBOT_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "QTelegramNetwork.h"
#include "Types/BCTypeChat.h"
#include "Types/BCTypeUpdate.h"
#include "Types/BCTypeUser.h"
#include "Types/BCTypeFile.h"
#include "Types/BCTypeMessage.h"
#include "Types/Reply/BCGenericReply.h"
#include "Types/Reply/BCReplyKeyboardMarkup.h"
#include "Types/Reply/BCReplyKeyboardHide.h"
#include "Types/Reply/BCReplyKeyboardRemove.h"
#include "Types/Reply/BCForceReply.h"
#include "Types/Reply/BCInlineKeyboardMarkup.h"
//-----------------------------------------------------------------------------
class QTelegramBot : public QObject
{
    Q_OBJECT

signals:
    void MessageSignal(const BCTypeMessage &Message);
    void UpdateSignal(const BCTypeUpdate &Update);

public:
    explicit QTelegramBot(const QString &token, bool updates = false, quint32 update_interval = 1000, quint32 polling_timeout = 0, QObject *parent = 0);
    ~QTelegramBot();

    enum ChatAction
    {
        Typing,
        UploadingPhoto,
        RecordingVideo,
        UploadingVideo,
        RecordingAudio,
        UploadingAudio,
        UploadingDocument,
        FindingLocation
    };

    BCTypeUser GetMe();
    bool SendMessage(QVariant chatId, QString text, bool markdown = false, bool disableWebPagePreview = false, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool ForwardMessage(QVariant chatId, quint32 fromChatId, quint32 messageId);
    bool SendPhoto(QVariant chatId, QFile *file, QString caption = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendPhoto(QVariant chatId, QString fileId, QString caption = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendAudio(QVariant chatId, QFile *file, qint64 duration = -1, QString performer = QString(), QString title = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendAudio(QVariant chatId, QString fileId, qint64 duration = -1, QString performer = QString(), QString title = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendDocument(QVariant chatId, QFile *file, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendDocument(QVariant chatId, QString fileId, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendSticker(QVariant chatId, QFile *file, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendSticker(QVariant chatId, QString fileId, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendVideo(QVariant chatId, QFile *file, qint64 duration = -1, QString caption = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendVideo(QVariant chatId, QString fileId, qint64 duration = -1, QString caption = QString(), qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendVoice(QVariant chatId, QFile *file, qint64 duration = -1, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendVoice(QVariant chatId, QString fileId, qint64 duration = -1, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendLocation(QVariant chatId, float latitude, float longitude, qint32 replyToMessageId = -1, const BCGenericReply &replyMarkup = BCGenericReply());
    bool SendChatAction(QVariant chatId, ChatAction action);
    bool AnswerCallbackQuery(QVariant callback_query_id, QString text = QString(), bool show_alert = false, QString url = QString(), quint32 cache_time = 0);
    QList<QList<BCTypePhotoSize>> GetUserProfilePhotos(quint32 userId, qint16 offset = -1, qint8 limit = -1);
    QList<BCTypeUpdate> GetUpdates(quint32 timeout, quint32 limit, quint32 offset);
    bool SetWebhook(QString url, QFile *certificate);
    BCTypeFile GetFile(QString fileId);

private:
    QTelegramNetwork *Network;

    bool SendPayload(QVariant chatId, QFile *filePayload, ParameterList Parameters, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint);
    bool SendPayload(QVariant chatId, QString textPayload, ParameterList Parameters, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint);

    QJsonObject JsonObjectFromByteArray(QByteArray ByteArray);
    QJsonArray JsonArrayFromByteArray(QByteArray ByteArray);
    bool ResponseOk(QByteArray ByteArray);

    void InternalGetUpdates();

private:
    QTimer *InternalUpdateTimer;
    quint32 UpdateInterval;
    quint32 UpdateOffset;
    quint32 PollingTimeout;
};
//-----------------------------------------------------------------------------
#endif
