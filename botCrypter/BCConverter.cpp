#include "BCConverter.h"
//-----------------------------------------------------------------------------
BCConverter::BCConverter(const QString &file_path)
    : FilePath(file_path)
{
    
}
//-----------------------------------------------------------------------------
BCConverter::~BCConverter()
{
    
}
//-----------------------------------------------------------------------------
bool BCConverter::Convert(QString &ErrorString)
{
    QFileInfo FileInfo(FilePath);
    QString NewFilePath = FileInfo.absolutePath() + "/" + FileInfo.baseName() + ".png";
    bool Ok = QImage(FilePath).save(NewFilePath, "PNG");
    if (Ok)
    {
        FilePath = NewFilePath;
    }
    else
    {
        ErrorString = "Error converting image to png.";
    }
    return Ok;
}
//-----------------------------------------------------------------------------
QString BCConverter::GetFilePath() const
{
    return FilePath;
}
//-----------------------------------------------------------------------------
