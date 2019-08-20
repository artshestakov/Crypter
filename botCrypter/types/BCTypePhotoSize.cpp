#include "StdAfx.h"
#include "BCTypePhotoSize.h"
//-----------------------------------------------------------------------------
BCTypePhotoSize::BCTypePhotoSize()
{

}
//-----------------------------------------------------------------------------
BCTypePhotoSize::BCTypePhotoSize(QJsonObject JsonObject)
{
    FileID = JsonObject.value("file_id").toString();
    Width = JsonObject.value("width").toInt();
    Height = JsonObject.value("height").toInt();
    FileSize = JsonObject.value("file_size").toInt();
}
//-----------------------------------------------------------------------------
BCTypePhotoSize::~BCTypePhotoSize()
{

}
//-----------------------------------------------------------------------------
