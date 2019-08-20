#pragma once
#ifndef _BCTYPEFILE_H_INCLUDED
#define _BCTYPEFILE_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeFile
{
public:
    BCTypeFile(const QString &file_id, qint64 file_size = -1, const QString &file_path = QString());
    ~BCTypeFile();

    QString FileID;
    qint64 FileSize;
    QString FilePath;
};
//-----------------------------------------------------------------------------
#endif
