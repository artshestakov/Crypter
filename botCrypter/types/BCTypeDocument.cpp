#include "StdAfx.h"
#include "BCTypePhotoSize.h"
#include "BCTypeDocument.h"
//-----------------------------------------------------------------------------
BCTypeDocument::BCTypeDocument()
{

}
//-----------------------------------------------------------------------------
BCTypeDocument::BCTypeDocument(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    PhotoSize = BCTypePhotoSize(JsonObject.value("thumb").toObject());
    FileName = JsonObject.value("file_name").toString();
    MimeType = JsonObject.value("mime_type").toString();
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypeDocument::~BCTypeDocument()
{

}
//-----------------------------------------------------------------------------
