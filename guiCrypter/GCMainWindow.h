#pragma once
#ifndef _GCMAINWINDOW_H_INCLUDED
#define _GCMAINWINDOW_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GCWidgetCrypt.h"
#include "GCWidgetDecrypt.h"
#include "GCPanelImage.h"
#include "GCThread.h"
//-----------------------------------------------------------------------------
class GCMainWindow : public QWidget
{
    Q_OBJECT

public:
    GCMainWindow(QWidget *parent = 0);
    virtual ~GCMainWindow();

private:
    void closeEvent(QCloseEvent *e);
    void ModeChanged();

private:
    QRadioButton *ButtonCrypt;
    QRadioButton *ButtonDecrypt;
    QStackedWidget *StackWidget;
    GCWidgetCrypt *WidgetCrypt;
    GCWidgetDecrypt *WidgetDecrypt;
};
//-----------------------------------------------------------------------------
#endif
