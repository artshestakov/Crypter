#include "StdAfx.h"
#include "BCTypeAudio.h"
//-----------------------------------------------------------------------------
BCTypeAudio::BCTypeAudio()
{

}
//-----------------------------------------------------------------------------
BCTypeAudio::BCTypeAudio(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    Duration = JsonObject.value("duration").toInt();
    Performer = JsonObject.value("performer").toString();
    Title = JsonObject.value("title").toString();
    MimeType = JsonObject.value("mime_type").toString();
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypeAudio::~BCTypeAudio()
{

}
//-----------------------------------------------------------------------------
