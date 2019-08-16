#pragma once
#ifndef _GCPANELIMAGE_H_INCLUDED
#define _GCPANELIMAGE_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------------------
class GCPanelImage : public QLabel
{
    Q_OBJECT

signals:
    void ImageChaned();

public:
    GCPanelImage(bool is_source, QWidget *parent = 0);
    virtual ~GCPanelImage();

    bool GetIsNull() const;
    QString GetFilePath() const;
    void mousePressEvent(QMouseEvent *e);
    void LoadPixmap(const QString &file_path);

private:
    void resizeEvent(QResizeEvent *e);
    void Copy();
    void Paste();
    void Clear();
    void Save();

private:
    bool IsSource;
    QPixmap PixmapNull;
    bool IsNull;
    QString FilePath;
};
//-----------------------------------------------------------------------------
#endif
