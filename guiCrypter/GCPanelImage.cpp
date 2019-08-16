#include "GCPanelImage.h"
//-----------------------------------------------------------------------------
GCPanelImage::GCPanelImage(bool is_source, QWidget *parent)
    : QLabel(parent),
    IsSource(is_source),
    PixmapNull(QPixmap(":Resources/NullImage.png")),
    IsNull(true)
{
    setCursor(Qt::PointingHandCursor);
    setAlignment(Qt::AlignCenter);
    Clear();
}
//-----------------------------------------------------------------------------
GCPanelImage::~GCPanelImage()
{

}
//-----------------------------------------------------------------------------
bool GCPanelImage::GetIsNull() const
{
    return IsNull;
}
//-----------------------------------------------------------------------------
QString GCPanelImage::GetFilePath() const
{
    return FilePath;
}
//-----------------------------------------------------------------------------
void GCPanelImage::mousePressEvent(QMouseEvent *e)
{
    QLabel::mousePressEvent(e);
    if (e->button() == Qt::LeftButton && IsSource)
    {
        QString SelectedPath = QFileDialog::getOpenFileName(parentWidget(), "Select image", QDir::homePath(), "Portable Network Graphics (*.png)");
        if (!SelectedPath.isEmpty())
        {
            setPixmap(QPixmap(SelectedPath, "PNG"));
            IsNull = false;
            FilePath = SelectedPath;
            emit ImageChaned();
        }
    }
    else
    {
        QMenu Menu;

        QAction *ActionCopy = Menu.addAction(QIcon(":Resources/Save.png"), "Copy", this, &GCPanelImage::Copy);
        ActionCopy->setEnabled(!GetIsNull());

        if (e->button() == Qt::RightButton && IsSource)
        {
            Menu.addAction(QIcon(":Resources/Clear.png"), "Clear", this, &GCPanelImage::Clear);
        }
        else if (e->button() == Qt::RightButton && !IsSource)
        {
            Menu.addAction(QIcon(":Resources/Save.png"), "Save", this, &GCPanelImage::Save);
            Menu.addAction(QIcon(":Resources/Paste.png"), "Paste", this, &GCPanelImage::Paste);
        }

        Menu.exec(e->globalPos());
    }
}
//-----------------------------------------------------------------------------
void GCPanelImage::LoadPixmap(const QString &file_path)
{
    setPixmap(QPixmap(file_path));
}
//-----------------------------------------------------------------------------
void GCPanelImage::resizeEvent(QResizeEvent *e)
{
    QLabel::resizeEvent(e);
    if (!GetIsNull())
    {
        setPixmap(pixmap()->scaled(width(), height(), Qt::KeepAspectRatio));
    }
}
//-----------------------------------------------------------------------------
void GCPanelImage::Copy()
{
    QApplication::clipboard()->setPixmap(*pixmap());
}
//-----------------------------------------------------------------------------
void GCPanelImage::Paste()
{
    setPixmap(QApplication::clipboard()->pixmap());
}
//-----------------------------------------------------------------------------
void GCPanelImage::Clear()
{
    setPixmap(PixmapNull);
    IsNull = true;
    emit ImageChaned();
}
//-----------------------------------------------------------------------------
void GCPanelImage::Save()
{
    QString Path = QFileDialog::getSaveFileName(parentWidget(), "Save", QDir::homePath(), "Portable Network Graphics (*.png)");
    if (!Path.isEmpty())
    {
        if (!pixmap()->save(Path, "PNG"))
        {
            QMessageBox::critical(parentWidget(), "Critical", "Error save image.", QMessageBox::Ok);
        }
    }
}
//-----------------------------------------------------------------------------
