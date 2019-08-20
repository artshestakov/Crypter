#include "StdAfx.h"
#include "BCTypePhotoSize.h"
#include "BCTypeSticker.h"
//-----------------------------------------------------------------------------
BCTypeSticker::BCTypeSticker()
{

}
//-----------------------------------------------------------------------------
BCTypeSticker::BCTypeSticker(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    Width = JsonObject.value("width").toInt();
    Height = JsonObject.value("height").toInt();
    PhotoSize = BCTypePhotoSize(JsonObject.value("thumb").toObject());
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypeSticker::~BCTypeSticker()
{

}
//-----------------------------------------------------------------------------
