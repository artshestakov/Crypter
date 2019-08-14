#pragma once
#ifndef _GCWIDGETDECRYPT_H_INCLUDED
#define _GCWIDGETDECRYPT_H_INCLUDED
//-----------------------------------------------------------------------------
#include "GCWidgetBase.h"
//-----------------------------------------------------------------------------
class GCWidgetDecrypt : public GCWidgetBase
{
    Q_OBJECT

public:
    GCWidgetDecrypt(QWidget *parent = 0);
    virtual ~GCWidgetDecrypt();

    void ButtonEnabled() override;
    void ButtonClicked() override;
    void ThreadStarted() override;
    void ThreadFinished() override;
    void Decrypted(bool Result, const QString &ErrorString, const QString &Message);
};
//-----------------------------------------------------------------------------
#endif
