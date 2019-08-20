#include "StdAfx.h"
#include "BCTypeVoice.h"
//-----------------------------------------------------------------------------
BCTypeVoice::BCTypeVoice()
{

}
//-----------------------------------------------------------------------------
BCTypeVoice::BCTypeVoice(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    Duration = JsonObject.value("duration").toInt();
    MimeType = JsonObject.value("mime_type").toString();
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypeVoice::~BCTypeVoice()
{

}
//-----------------------------------------------------------------------------
