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
    bool Ok = true;
    QString ImageFormat = QImageReader(FilePath).format();
    if (ImageFormat.toLower() != "png")
    {
        QFileInfo FileInfo(FilePath);
        QString NewFilePath = FileInfo.absolutePath() + "/" + FileInfo.baseName() + ".png";
        Ok = QImage(FilePath).save(NewFilePath, "PNG");
        if (Ok)
        {
            FilePath = NewFilePath;
        }
        else
        {
            ErrorString = "Error converting " + ImageFormat + " to png.";
        }
    }
    return Ok;
}
//-----------------------------------------------------------------------------
QString BCConverter::GetFilePath() const
{
    return FilePath;
}
//-----------------------------------------------------------------------------
