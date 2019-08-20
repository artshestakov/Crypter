#include "StdAfx.h"
#include "BCTypeVideo.h"
//-----------------------------------------------------------------------------
BCTypeVideo::BCTypeVideo()
{

}
//-----------------------------------------------------------------------------
BCTypeVideo::BCTypeVideo(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    Width = JsonObject.value("width").toInt();
    Height = JsonObject.value("height").toInt();
    Duration = JsonObject.value("duration").toInt();
    PhotoSize = BCTypePhotoSize(JsonObject.value("thumb").toObject());
    MimeType = JsonObject.value("mime_type").toString();
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypeVideo::~BCTypeVideo()
{

}
//-----------------------------------------------------------------------------
