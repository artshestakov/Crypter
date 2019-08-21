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
    ParameterList params;
    if (markdown)
    {
        params.insert("parse_mode", QHttpParameter("Markdown"));
    }
    if (disableWebPagePreview)
    {
        params.insert("disable_web_page_preview", QHttpParameter(disableWebPagePreview));
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

    ParameterList params;
    params.insert("chat_id", QHttpParameter(chatId));
    params.insert("from_chat_id", QHttpParameter(fromChatId));
    params.insert("message_id", QHttpParameter(messageId));
    return ResponseOk(Network->Request(ENDPOINT_FORWARD_MESSAGE, params, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPhoto(QVariant chatId, QFile *file, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    if (!caption.isEmpty())
    {
        params.insert("caption", QHttpParameter(caption));
    }
    return SendPayload(chatId, file, params, replyToMessageId, replyMarkup, "photo", ENDPOINT_SEND_PHOTO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPhoto(QVariant chatId, QString fileId, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    if (!caption.isEmpty())
    {
        params.insert("caption", QHttpParameter(caption));
    }
    return SendPayload(chatId, fileId, params, replyToMessageId, replyMarkup, "photo", ENDPOINT_SEND_PHOTO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendAudio(QVariant chatId, QFile *file, qint64 duration, QString performer, QString title, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    if (duration >= 0)
    {
        params.insert("duration", QHttpParameter(duration));
    }
    if (!performer.isEmpty())
    {
        params.insert("performer", QHttpParameter(performer));
    }
    if (!title.isEmpty())
    {
        params.insert("title", QHttpParameter(title));
    }
    return SendPayload(chatId, file, params, replyToMessageId, replyMarkup, "audio", ENDPOINT_SEND_AUDIO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendAudio(QVariant chatId, QString fileId, qint64 duration, QString performer, QString title, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    if (duration >= 0)
    {
        params.insert("duration", QHttpParameter(duration));
    }
    if (!performer.isEmpty())
    {
        params.insert("performer", QHttpParameter(performer));
    }
    if (!title.isEmpty())
    {
        params.insert("title", QHttpParameter(title));
    }
    return SendPayload(chatId, fileId, params, replyToMessageId, replyMarkup, "audio", ENDPOINT_SEND_AUDIO);
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
    ParameterList params;
    params.insert("duration", QHttpParameter(duration));
    params.insert("caption", QHttpParameter(caption));
    return SendPayload(chatId, file, params, replyToMessageId, replyMarkup, "video", ENDPOINT_SEND_VIDEO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVideo(QVariant chatId, QString fileId, qint64 duration, QString caption, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    params.insert("duration", QHttpParameter(duration));
    params.insert("caption", QHttpParameter(caption));
    return SendPayload(chatId, fileId, params, replyToMessageId, replyMarkup, "video", ENDPOINT_SEND_VIDEO);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVoice(QVariant chatId, QFile *file, qint64 duration, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    params.insert("duration", QHttpParameter(duration));
    return SendPayload(chatId, file, params, replyToMessageId, replyMarkup, "voice", ENDPOINT_SEND_VOICE);
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendVoice(QVariant chatId, QString fileId, qint64 duration, qint32 replyToMessageId, const BCGenericReply &replyMarkup)
{
    ParameterList params;
    params.insert("duration", QHttpParameter(duration));
    return SendPayload(chatId, fileId, params, replyToMessageId, replyMarkup, "voice", ENDPOINT_SEND_VOICE);
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

    ParameterList params;
    params.insert("chat_id", QHttpParameter(chatId));
    params.insert("latitude", QHttpParameter(latitude));
    params.insert("longitude", QHttpParameter(longitude));
    if (replyToMessageId >= 0)
    {
        params.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }
    return ResponseOk(Network->Request(ENDPOINT_SEND_LOCATION, params, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendChatAction(QVariant chatId, QTelegramBot::ChatAction action)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    ParameterList params;
    params.insert("chat_id", QHttpParameter(chatId));
    switch (action)
    {
    case Typing: params.insert("action", QHttpParameter("typing")); break;
    case UploadingPhoto: params.insert("action", QHttpParameter("upload_photo")); break;
    case RecordingVideo: params.insert("action", QHttpParameter("record_video")); break;
    case UploadingVideo: params.insert("action", QHttpParameter("upload_video")); break;
    case RecordingAudio: params.insert("action", QHttpParameter("record_audio")); break;
    case UploadingAudio: params.insert("action", QHttpParameter("upload_audio")); break;
    case UploadingDocument: params.insert("action", QHttpParameter("upload_document")); break;
    case FindingLocation: params.insert("action", QHttpParameter("find_location")); break;
    default:
        return false;
    }
    return ResponseOk(Network->Request(ENDPOINT_SEND_CHAT_ACTION, params, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::AnswerCallbackQuery(QVariant callback_query_id, QString text, bool show_alert, QString url, quint32 cache_time)
{
    ParameterList params;
    params.insert("callback_query_id", QHttpParameter(callback_query_id));
    if (!text.isEmpty())
    {
        params.insert("text", QHttpParameter(text));
    }
    if (show_alert)
    {
        params.insert("show_alert", QHttpParameter(show_alert));
    }
    if (!url.isEmpty())
    {
        params.insert("url", QHttpParameter(url));
    }
    if (cache_time)
    {
        params.insert("cache_time", QHttpParameter(cache_time));
    }
    return ResponseOk(Network->Request(ENDPOINT_ANSWER_CALLBACK_QUERY, params, QTelegramNetwork::POST));
}
//-----------------------------------------------------------------------------
QList<QList<BCTypePhotoSize>> QTelegramBot::getUserProfilePhotos(quint32 userId, qint16 offset, qint8 limit)
{
    ParameterList params;
    params.insert("user_id", QHttpParameter(userId));
    if (offset > -1)
    {
        params.insert("offset", QHttpParameter(offset));
    }
    if (limit > -1)
    {
        params.insert("limit", QHttpParameter(limit));
    }

    QJsonObject JsonObject = JsonObjectFromByteArray(Network->Request(ENDPOINT_GET_USER_PROFILE_PHOTOS, params, QTelegramNetwork::GET));

    QList<QList<BCTypePhotoSize>> ret;
    QList<BCTypePhotoSize> photo;
    foreach(QJsonValue JsonValue, JsonObject.value("photos").toArray())
    {
        photo = QList<BCTypePhotoSize>();
        foreach(QJsonValue p, JsonValue.toArray())
        {
            BCTypePhotoSize ps;
            ps.FileID = p.toObject().value("file_id").toString();
            ps.Width = p.toObject().value("width").toInt();
            ps.Height = p.toObject().value("height").toInt();
            if (p.toObject().contains("file_size"))
            {
                ps.FileSize = p.toObject().value("file_size").toInt();
            }
            photo.append(ps);
        }
        ret.append(photo);
    }
    return ret;
}
//-----------------------------------------------------------------------------
QList<BCTypeUpdate> QTelegramBot::getUpdates(quint32 timeout, quint32 limit, quint32 offset)
{
    ParameterList params;
    params.insert("offset", QHttpParameter(offset));
    params.insert("limit", QHttpParameter(limit));
    params.insert("timeout", QHttpParameter(timeout));
    QJsonArray JsonArray = JsonArrayFromByteArray(Network->Request(ENDPOINT_GET_UPDATES, params, QTelegramNetwork::GET));

    QList<BCTypeUpdate> Result = QList<BCTypeUpdate>();
    foreach(QJsonValue JsonValue, JsonArray)
    {
        Result.append(BCTypeUpdate(JsonValue.toObject()));
    }
    return Result;
}
//-----------------------------------------------------------------------------
bool QTelegramBot::setWebhook(QString url, QFile *certificate)
{
    ParameterList params;
    params.insert("url", QHttpParameter(url));

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
    params.insert("certificate", QHttpParameter(ByteArray, true, MimeDatabase.mimeTypeForData(ByteArray).name(), certificate->fileName()));
    return ResponseOk(Network->Request(ENDPOINT_SET_WEBHOOK, params, QTelegramNetwork::UPLOAD));
}
//-----------------------------------------------------------------------------
BCTypeFile QTelegramBot::getFile(QString fileId)
{
    ParameterList params;
    params.insert("file_id", QHttpParameter(fileId));
    QJsonObject JsonObject = JsonObjectFromByteArray(Network->Request(ENDPOINT_GET_FILE, params, QTelegramNetwork::GET));
    return BCTypeFile(JsonObject.value("file_id").toString(), JsonObject.value("file_size").toInt(-1), JsonObject.value("file_path").toString());
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPayload(QVariant chatId, QFile *filePayload, ParameterList params, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    params.insert("chat_id", QHttpParameter(chatId));

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
    params.insert(payloadField, QHttpParameter(data, true, db.mimeTypeForData(data).name(), filePayload->fileName()));

    if (replyToMessageId >= 0)
    {
        params.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }
    if (replyMarkup.IsValid())
    {
        params.insert("reply_markup", QHttpParameter(replyMarkup.Serialize()));
    }
    return ResponseOk(Network->Request(endpoint, params, QTelegramNetwork::UPLOAD));
}
//-----------------------------------------------------------------------------
bool QTelegramBot::SendPayload(QVariant chatId, QString textPayload, ParameterList params, qint32 replyToMessageId, const BCGenericReply &replyMarkup, QString payloadField, QString endpoint)
{
    if (chatId.type() != QVariant::String && chatId.type() != QVariant::Int)
    {
        qCritical("Please provide a QString or int as chatId");
        return false;
    }

    params.insert("chat_id", QHttpParameter(chatId));
    params.insert(payloadField, QHttpParameter(textPayload));

    if (replyToMessageId >= 0)
    {
        params.insert("reply_to_message_id", QHttpParameter(replyToMessageId));
    }

    if (replyMarkup.IsValid())
    {
        params.insert("reply_markup", QHttpParameter(replyMarkup.Serialize()));
    }

    return ResponseOk(Network->Request(endpoint, params, QTelegramNetwork::POST));
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
    QList<BCTypeUpdate> Updates = getUpdates(PollingTimeout, 50, UpdateOffset);
    foreach(BCTypeUpdate Update, Updates)
    {
        UpdateOffset = (Update.ID >= UpdateOffset ? Update.ID + 1 : UpdateOffset);
        emit MessageSignal(Update.Message);
        emit UpdateSignal(Update);
    }
    InternalUpdateTimer->start(UpdateInterval);
}
//-----------------------------------------------------------------------------
