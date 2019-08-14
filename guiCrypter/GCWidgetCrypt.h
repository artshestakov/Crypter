#pragma once
#ifndef _GCWIDGETCRYPT_H_INCLUDED
#define _GCWIDGETCRYPT_H_INCLUDED
//-----------------------------------------------------------------------------
#include "GCWidgetBase.h"
#include "GCPanelImage.h"
#include "GCThread.h"
//-----------------------------------------------------------------------------
class GCWidgetCrypt : public GCWidgetBase
{
    Q_OBJECT

public:
    GCWidgetCrypt(QWidget *parent = 0);
    virtual ~GCWidgetCrypt();

private:
    void ButtonEnabled() override;
    void ButtonClicked() override;
    void ThreadStarted() override;
    void ThreadFinished() override;
    void Crypted(bool Result, const QString &ErrorString, const QString &PathOutput);

private:
    GCPanelImage *ImageLeft;
    QTextEdit *TextEdit;
    QPushButton *Button;
    GCPanelImage *ImageRight;
    GCThread *Thread;
};
//-----------------------------------------------------------------------------
#endif
