#include "QTelegramBot.h"
//-----------------------------------------------------------------------------
QTelegramBot::QTelegramBot(const QString &token, bool updates, quint32 update_interval, quint32 polling_timeout, QObject *parent)
    : QObject(parent),
    Network(new QTelegramNetwork(token)),
    InternalUpdateTimer(new QTimer(this)),
    UpdateInterval(update_interval),
    UpdateOffset(0),
    PollingTimeout(polling_timeout)
{
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
    if (updates)
    {
        InternalUpdateTimer->setSingleShot(true);
        connect(InternalUpdateTimer, &QTimer::timeout, this, &QTelegramBot::InternalGetUpdates);
        InternalGetUpdates();
    }
}
//-----------------------------------------------------------------------------
QTelegramBot::~QTelegramBot()
{
    delete Network;
}
//-----------------------------------------------------------------------------
BCTypeUser QTelegramBot::GetMe()
{
    QJsonObject JsonObject = JsonObjectFromByteArray(Network->Request(ENDPOINT_GET_ME, ParameterList(), QTelegramNetwork::GET));

    BCTypeUser Result;
    Result.ID = JsonObject.value("id").toInt();
    Result.FirstName = JsonObject.value("first_name").toString();
    Result.LastName = JsonObject.value("last_name").toString();
    Result.UserName = JsonObject.value("username").toString();
    if (Result.ID == 0 || Result.FirstName.isEmpty())
    {
        qCritical("%s", qPrintable("Got invalid user in " + QString(ENDPOINT_GET_ME)));
        return BCTypeUser();
    }
    return Result;
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendMessage(QVariant chatId, QString text, bool markdown, bool disableWebPagePreview, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    if (markdown)
    {
        Parameters.insert("parse_mode", QHttpParameter("Markdown"));
    }
    if (disableWebPagePreview)
    {
        Parameters.insert("disable_web_page_preview", QHttpParameter(disableWebPagePreview));
    }
    return SendPayload(chatId, text, ParameterList(), replyToMessageId, replyMarkup, "text", ENDPOINT_SEND_MESSAGE);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::ForwardMessage(QVariant chatId, quint32 fromChatId, quint32 messageId)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    ParameterList Parameters;
    Parameters.insert("chat_id", QHttpParameter(chatId));
    Parameters.insert("from_chat_id", QHttpParameter(fromChatId));
    Parameters.insert("message_id", QHttpParameter(messageId));
    return ResponseOk(Network->Request(ENDPOINT_FORWARD_MESSAGE, Parameters, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPhoto(QVariant chatId, QFile *file, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    if (!caption.isEmpty())
    {
        Parameters.insert("caption", QHttpParameter(caption));
    }
    return SendPayload(chatId, file, Parameters, replyToMessageId, replyMarkup, "photo", ENDPOINT_SEND_PHOTO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPhoto(QVariant chatId, QString fileId, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    if (!caption.isEmpty())
    {
        Parameters.insert("caption", QHttpParameter(caption));
    }
    return SendPayload(chatId, fileId, Parameters, replyToMessageId, replyMarkup, "photo", ENDPOINT_SEND_PHOTO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendAudio(QVariant chatId, QFile *file, qint64 duration, QString performer, QString title, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    if (duration >= 0)
    {
        Parameters.insert("duration", QHttpParameter(duration));
    }
    if (!performer.isEmpty())
    {
        Parameters.insert("performer", QHttpParameter(performer));
    }
    if (!title.isEmpty())
    {
        Parameters.insert("title", QHttpParameter(title));
    }
    return SendPayload(chatId, file, Parameters, replyToMessageId, replyMarkup, "audio", ENDPOINT_SEND_AUDIO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendAudio(QVariant chatId, QString fileId, qint64 duration, QString performer, QString title, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    if (duration >= 0)
    {
        Parameters.insert("duration", QHttpParameter(duration));
    }
    if (!performer.isEmpty())
    {
        Parameters.insert("performer", QHttpParameter(performer));
    }
    if (!title.isEmpty())
    {
        Parameters.insert("title", QHttpParameter(title));
    }
    return SendPayload(chatId, fileId, Parameters, replyToMessageId, replyMarkup, "audio", ENDPOINT_SEND_AUDIO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendDocument(QVariant chatId, QFile *file, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    return SendPayload(chatId, file, ParameterList(), replyToMessageId, replyMarkup, "document", ENDPOINT_SEND_DOCUMENT);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendDocument(QVariant chatId, QString fileId, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    return SendPayload(chatId, fileId, ParameterList(), replyToMessageId, replyMarkup, "document", ENDPOINT_SEND_DOCUMENT);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendSticker(QVariant chatId, QFile *file, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    return SendPayload(chatId, file, ParameterList(), replyToMessageId, replyMarkup, "sticker", ENDPOINT_SEND_STICKER);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendSticker(QVariant chatId, QString fileId, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    return SendPayload(chatId, fileId, ParameterList(), replyToMessageId, replyMarkup, "sticker", ENDPOINT_SEND_STICKER);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVideo(QVariant chatId, QFile *file, qint64 duration, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    Parameters.insert("duration", QHttpParameter(duration));
    Parameters.insert("caption", QHttpParameter(caption));
    return SendPayload(chatId, file, Parameters, replyToMessageId, replyMarkup, "video", ENDPOINT_SEND_VIDEO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVideo(QVariant chatId, QString fileId, qint64 duration, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    Parameters.insert("duration", QHttpParameter(duration));
    Parameters.insert("caption", QHttpParameter(caption));
    return SendPayload(chatId, fileId, Parameters, replyToMessageId, replyMarkup, "video", ENDPOINT_SEND_VIDEO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVoice(QVariant chatId, QFile *file, qint64 duration, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    Parameters.insert("duration", QHttpParameter(duration));
    return SendPayload(chatId, file, Parameters, replyToMessageId, replyMarkup, "voice", ENDPOINT_SEND_VOICE);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVoice(QVariant chatId, QString fileId, qint64 duration, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList Parameters;
    Parameters.insert("duration", QHttpParameter(duration));
    return SendPayload(chatId, fileId, Parameters, replyToMessageId, replyMarkup, "voice", ENDPOINT_SEND_VOICE);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendLocation(QVariant chatId, float latitude, float longitude, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    Q_UNUSED(replyMarkup);
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    ParameterList Parameters;
    Parameters.insert("chat_id", QHttpParameter(chatId));
    Parameters.insert("latitude", QHttpParameter(latitude));
    Parameters.insert("longitude", QHttpParameter(longitude));
    if (replyToMessageId >= 0)
    {
        Parameters.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }
    return ResponseOk(Network->Request(ENDPOINT_SEND_LOCATION, Parameters, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendChatAction(QVariant chatId, QTelegramBot::ChatAction action)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    ParameterList Parameters;
    Parameters.insert("chat_id", QHttpParameter(chatId));
    switch (action)
    {
    case Typing: Parameters.insert("action", QHttpParameter("typing")); break;
    case UploadingPhoto: Parameters.insert("action", QHttpParameter("upload_photo")); break;
    case RecordingVideo: Parameters.insert("action", QHttpParameter("record_video")); break;
    case UploadingVideo: Parameters.insert("action", QHttpParameter("upload_video")); break;
    case RecordingAudio: Parameters.insert("action", QHttpParameter("record_audio")); break;
    case UploadingAudio: Parameters.insert("action", QHttpParameter("upload_audio")); break;
    case UploadingDocument: Parameters.insert("action", QHttpParameter("upload_document")); break;
    case FindingLocation: Parameters.insert("action", QHttpParameter("find_location")); break;
    default:
        return false;
    }
    return ResponseOk(Network->Request(ENDPOINT_SEND_CHAT_ACTION, Parameters, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::AnswerCallbackQuery(QVariant callback_query_id, QString text, bool show_alert, QString url, quint32 cache_time)
{
    ParameterList Parameters;
    Parameters.insert("callback_query_id", QHttpParameter(callback_query_id));
    if (!text.isEmpty())
    {
        Parameters.insert("text", QHttpParameter(text));
    }
    if (show_alert)
    {
        Parameters.insert("show_alert", QHttpParameter(show_alert));
    }
    if (!url.isEmpty())
    {
        Parameters.insert("url", QHttpParameter(url));
    }
    if (cache_time)
    {
        Parameters.insert("cache_time", QHttpParameter(cache_time));
    }
    return ResponseOk(Network->Request(ENDPOINT_ANSWER_CALLBACK_QUERY, Parameters, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
QList<QList<BCTypePhotoSize>> QTelegramBot::GetUserProfilePhotos(quint32 userId, qint16 offset, qint8 limit)
{
    ParameterList Parameters;
    Parameters.insert("user_id", QHttpParameter(userId));
    if (offset > -1)
    {
        Parameters.insert("offset", QHttpParameter(offset));
    }
    if (limit > -1)
    {
        Parameters.insert("limit", QHttpParameter(limit));
    }

    QJsonObject JsonObject = JsonObjectFromByteArray(Network->Request(ENDPOINT_GET_USER_PROFILE_PHOTOS, Parameters, QTelegramNetwork::GET));

    QList<QList<BCTypePhotoSize>> Result;
    QList<BCTypePhotoSize> Photo;
    foreach(QJsonValue JsonValue, JsonObject.value("photos").toArray())
    {
        foreach(QJsonValue JsonValue, JsonValue.toArray())
        {
            BCTypePhotoSize PhotoSize;
            PhotoSize.FileID = JsonValue.toObject().value("file_id").toString();
            PhotoSize.Width = JsonValue.toObject().value("width").toInt();
            PhotoSize.Height = JsonValue.toObject().value("height").toInt();
            if (JsonValue.toObject().contains("file_size"))
            {
                PhotoSize.FileSize = JsonValue.toObject().value("file_size").toInt();
            }
            Photo.append(PhotoSize);
        }
        Result.append(Photo);
    }
    return Result;
}
//-----------------------------------------------------------------------------
QList<BCTypeUpdate> QTelegramBot::GetUpdates(quint32 timeout, quint32 limit, quint32 offset)
{
    ParameterList Parameters;
    Parameters.insert("offset", QHttpParameter(offset));
    Parameters.insert("limit", QHttpParameter(limit));
    Parameters.insert("timeout", QHttpParameter(timeout));
    QJsonArray JsonArray = JsonArrayFromByteArray(Network->Request(ENDPOINT_GET_UPDATES, Parameters, QTelegramNetwork::GET));

    QList<BCTypeUpdate> Result;
    foreach(QJsonValue JsonValue, JsonArray)
    {
        Result.append(BCTypeUpdate(JsonValue.toObject()));
    }
    return Result;
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SetWebhook(QString url, QFile *certificate)
{
    ParameterList Parameters;
    Parameters.insert("url", QHttpParameter(url));

    QMimeDatabase MimeDatabase;
    bool openedFile = false;
    if (!certificate->isOpen())
    {
        if (!certificate->open(QFile::ReadOnly))
        {
            qCritical("Could not open file %s [%s]", qPrintable(certificate->fileName()), qPrintable(certificate->errorString()));
            return false;
        }
        openedFile = true;
    }
    QByteArray ByteArray = certificate->readAll();
    if (openedFile)
    {
        certificate->close();
    }
    Parameters.insert("certificate", QHttpParameter(ByteArray, true, MimeDatabase.mimeTypeForData(ByteArray).name(), certificate->fileName()));
    return ResponseOk(Network->Request(ENDPOINT_SET_WEBHOOK, Parameters, QTelegramNetwork::UPLOAD));
}
//-----------------------------------------------------------------------------
BCTypeFile QTelegramBot::GetFile(QString fileId)
{
    ParameterList Parameters;
    Parameters.insert("file_id", QHttpParameter(fileId));
    QJsonObject JsonObject = JsonObjectFromByteArray(Network->Request(ENDPOINT_GET_FILE, Parameters, QTelegramNetwork::GET));
    return BCTypeFile(JsonObject.value("file_id").toString(), JsonObject.value("file_size").toInt(-1), JsonObject.value("file_path").toString());
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPayload(QVariant chatId, QFile *filePayload, ParameterList Parameters, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    Parameters.insert("chat_id", QHttpParameter(chatId));

    QMimeDatabase db;
    bool openedFile = false;
    if (!filePayload->isOpen())
    {
        if (!filePayload->open(QFile::ReadOnly))
        {
            qCritical("Could not open file %s [%s]", qPrintable(filePayload->fileName()), qPrintable(filePayload->errorString()));
            return false;
        }
        openedFile = true;
    }
    QByteArray data = filePayload->readAll();
    if (openedFile)
    {
        filePayload->close();
    }
    Parameters.insert(payloadField, QHttpParameter(data, true, db.mimeTypeForData(data).name(), filePayload->fileName()));

    if (replyToMessageId >= 0)
    {
        Parameters.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }
    if (replyMarkup.IsValid())
    {
        Parameters.insert("reply_markup", QHttpParameter(replyMarkup.Serialize()));
    }
    return ResponseOk(Network->Request(endpoint, Parameters, QTelegramNetwork::UPLOAD));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPayload(QVariant chatId, QString textPayload, ParameterList Parameters, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    Parameters.insert("chat_id", QHttpParameter(chatId));
    Parameters.insert(payloadField, QHttpParameter(textPayload));

    if (replyToMessageId >= 0)
    {
        Parameters.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }

    if (replyMarkup.IsValid())
    {
        Parameters.insert("reply_markup", QHttpParameter(replyMarkup.Serialize()));
    }

    return ResponseOk(Network->Request(endpoint, Parameters, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
QJsonObject QTelegramBot::JsonObjectFromByteArray(QByteArray Bytearray)
{
    QJsonObject JsonObject = QJsonDocument::fromJson(Bytearray).object();
    if (JsonObject.isEmpty())
    {
        qCritical("Got an empty response object");
        return JsonObject;
    }
    if (JsonObject.value("ok").toBool() != true)
    {
        qWarning("Result is not Ok");
        return JsonObject;
    }
    return JsonObject.value("result").toObject();
}
//-----------------------------------------------------------------------------
QJsonArray QTelegramBot::JsonArrayFromByteArray(QByteArray ByteArray)
{
    QJsonObject JsonObject = QJsonDocument::fromJson(ByteArray).object();
    if (JsonObject.isEmpty())
    {
        qCritical("Got an empty response object");
        return QJsonArray();
    }
    if (JsonObject.value("ok").toBool() != true)
    {
        qWarning("Result is not Ok");
        return QJsonArray();
    }
    return JsonObject.value("result").toArray();
}
//-----------------------------------------------------------------------------
bool QTelegramBot::ResponseOk(QByteArray ByteArray)
{
    QJsonObject obj = QJsonDocument::fromJson(ByteArray).object();
    return (!obj.isEmpty() && obj.value("ok").toBool() == true);
}
//-----------------------------------------------------------------------------
void QTelegramBot::InternalGetUpdates()
{
    QList<BCTypeUpdate> Updates = GetUpdates(PollingTimeout, 50, UpdateOffset);
    for (const BCTypeUpdate &Update : Updates)
    {
        UpdateOffset = (Update.ID >= UpdateOffset ? Update.ID + 1 : UpdateOffset);
        emit MessageSignal(Update.Message);
        emit UpdateSignal(Update);
    }
    InternalUpdateTimer->start(UpdateInterval);
}
//-----------------------------------------------------------------------------
