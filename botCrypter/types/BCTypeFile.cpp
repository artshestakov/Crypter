#include "StdAfx.h"
#include "BCTypeFile.h"
//-----------------------------------------------------------------------------
BCTypeFile::BCTypeFile(const QString &file_id, qint64 file_size, const QString &file_path)
    : FileID(file_id),
    FileSize(file_size),
    FilePath(file_path)
{

}
//-----------------------------------------------------------------------------
BCTypeFile::~BCTypeFile()
{

}
//-----------------------------------------------------------------------------
